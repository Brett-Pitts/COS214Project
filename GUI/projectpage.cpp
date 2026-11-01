#include "projectpage.h"
#include "ui_projectpage.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStringList>
#include <QTreeWidgetItem>

#include "ApplicationInterface.h"
#include "Project.h"
#include "WorkUnit.h"

namespace {

const int NodeRole = Qt::UserRole;

QString toQ(const std::string& text){
    return QString::fromStdString(text);
}

QString prettyState(const std::string& state){
    QString text = toQ(state);
    text.replace(QRegularExpression("([a-z])([A-Z])"), "\\1 \\2");
    return text;
}

QString prerequisiteText(WorkNode* node){
    QStringList names;
    for(const std::string& name : node->getPrerequisites()) names << toQ(name);
    return names.join(", ");
}

WorkNode* nodeOf(QTreeWidgetItem* item){
    if(item == nullptr) return nullptr;
    return reinterpret_cast<WorkNode*>(static_cast<quintptr>(item->data(0, NodeRole).toULongLong()));
}

}

ProjectPage::ProjectPage(QWidget *parent): QWidget(parent), ui(new Ui::ProjectPage), app(nullptr){
    ui->setupUi(this);
    ui->projectTree->setColumnWidth(0, 200);
    connect(ui->backButton, &QPushButton::clicked, this, &ProjectPage::backRequested);

    ui->professionCombo->setEditable(true);

    connect(ui->projectTree, &QTreeWidget::itemSelectionChanged, this, &ProjectPage::onSelectionChanged);
    connect(ui->newProjectButton, &QPushButton::clicked, this, &ProjectPage::addProject);
    connect(ui->addSubProjectButton, &QPushButton::clicked, this, &ProjectPage::addSubProject);
    connect(ui->addWorkUnitButton, &QPushButton::clicked, this, &ProjectPage::addWorkUnit);
    connect(ui->deleteNodeButton, &QPushButton::clicked, this, &ProjectPage::deleteNode);
    connect(ui->saveChangesButton, &QPushButton::clicked, this, &ProjectPage::saveChanges);
    connect(ui->addPrerequisiteButton, &QPushButton::clicked, this, &ProjectPage::addPrerequisite);
    connect(ui->removePrerequisiteButton, &QPushButton::clicked, this, &ProjectPage::removePrerequisite);
}

ProjectPage::~ProjectPage(){
    delete ui;
}

void ProjectPage::setApplication(ApplicationInterface* application){
    app = application;
    refresh();
}

WorkNode* ProjectPage::selectedNode() const{
    QList<QTreeWidgetItem*> picked = ui->projectTree->selectedItems();
    if(picked.isEmpty()) return nullptr;
    return nodeOf(picked.first());
}

void ProjectPage::addChildItems(QTreeWidgetItem* parentItem, WorkNode* node){
    for(int i = 0;; i++){
        WorkNode* child = node->getChild(i);
        if(child == nullptr) break;

        QTreeWidgetItem* item = parentItem != nullptr ? new QTreeWidgetItem(parentItem)
                                                      : new QTreeWidgetItem(ui->projectTree);
        bool isUnit = dynamic_cast<WorkUnit*>(child) != nullptr;
        item->setText(0, toQ(child->getName()));
        item->setText(1, isUnit ? "Work Unit" : "Project");
        item->setText(2, prettyState(child->getStateName()));
        item->setText(3, prerequisiteText(child));
        item->setData(0, NodeRole, static_cast<qulonglong>(reinterpret_cast<quintptr>(child)));
        items.insert(child, item);
        addChildItems(item, child);
    }
}

void ProjectPage::refresh(){
    if(app == nullptr) return;

    WorkNode* keep = selectedNode();

    ui->projectTree->blockSignals(true);
    ui->projectTree->clear();
    items.clear();
    addChildItems(nullptr, app->getWorkflow());
    ui->projectTree->expandAll();
    QTreeWidgetItem* again = items.value(keep, nullptr);
    if(again != nullptr) ui->projectTree->setCurrentItem(again);
    ui->projectTree->blockSignals(false);

    fillCombos();
    onSelectionChanged();
}

void ProjectPage::selectNode(WorkNode* node){
    QTreeWidgetItem* item = items.value(node, nullptr);
    if(item != nullptr) ui->projectTree->setCurrentItem(item);
}

void ProjectPage::fillCombos(){
    QStringList professions;
    for(const std::string& p : app->getProfessions()) professions << toQ(p);
    for(WorkUnit* unit : app->getWorkUnits()){
        QString p = toQ(unit->getRequiredProfession());
        if(!p.isEmpty() && !professions.contains(p)) professions << p;
    }
    ui->professionCombo->clear();
    ui->professionCombo->addItems(professions);
    ui->professionCombo->setEditText("");
}

void ProjectPage::onSelectionChanged(){
    if(app == nullptr) return;

    WorkNode* node = selectedNode();
    Project* project = dynamic_cast<Project*>(node);
    WorkUnit* unit = dynamic_cast<WorkUnit*>(node);

    ui->deleteNodeButton->setEnabled(node != nullptr);
    ui->addSubProjectButton->setEnabled(project != nullptr);
    ui->addWorkUnitButton->setEnabled(project != nullptr);
    ui->detailsBox->setEnabled(node != nullptr);
    ui->professionCombo->setEnabled(unit != nullptr);

    ui->prerequisiteList->clear();
    ui->prerequisiteCombo->clear();

    if(node == nullptr){
        ui->nodeNameEdit->clear();
        ui->nodeTypeLabel->setText("-");
        ui->nodeStateLabel->setText("-");
        ui->nodeAssigneeLabel->setText("-");
        ui->maxRetriesSpin->setValue(0);
        return;
    }

    ui->nodeNameEdit->setText(toQ(node->getName()));
    ui->nodeTypeLabel->setText(unit != nullptr ? "Work Unit" : "Project");
    ui->nodeStateLabel->setText(prettyState(node->getStateName()));
    ui->maxRetriesSpin->setValue(node->getMaxRetries());

    const std::map<std::string, std::string>& assignments = app->getAssignments();
    std::map<std::string, std::string>::const_iterator assigned = assignments.find(node->getName());
    ui->nodeAssigneeLabel->setText(assigned != assignments.end() ? toQ(assigned->second) : QString("-"));

    for(const std::string& name : node->getPrerequisites()) ui->prerequisiteList->addItem(toQ(name));

    for(WorkNode* other : app->getWorkNodes()){
        if(other == app->getWorkflow() || other == node) continue;
        const std::vector<std::string>& existing = node->getPrerequisites();
        bool already = false;
        for(const std::string& name : existing) if(name == other->getName()) already = true;
        if(!already) ui->prerequisiteCombo->addItem(toQ(other->getName()));
    }

    ui->professionCombo->setEditText("");
    if(unit != nullptr) ui->professionCombo->setEditText(toQ(unit->getRequiredProfession()));
}

void ProjectPage::addProject(){
    if(app == nullptr) return;
    bool ok = false;
    QString name = QInputDialog::getText(this, "New project", "Project name:", QLineEdit::Normal, "", &ok).trimmed();
    if(!ok || name.isEmpty()) return;
    Project* created = app->createProject(nullptr, name.toStdString());
    if(created == nullptr){
        QMessageBox::warning(this, "New project", "That name is already used. Names must be unique.");
        return;
    }
    refresh();
    selectNode(created);
}

void ProjectPage::addSubProject(){
    Project* parent = dynamic_cast<Project*>(selectedNode());
    if(app == nullptr || parent == nullptr) return;
    bool ok = false;
    QString name = QInputDialog::getText(this, "New sub-project", "Sub-project name:", QLineEdit::Normal, "", &ok).trimmed();
    if(!ok || name.isEmpty()) return;
    Project* created = app->createProject(parent, name.toStdString());
    if(created == nullptr){
        QMessageBox::warning(this, "New sub-project", "That name is already used. Names must be unique.");
        return;
    }
    refresh();
    selectNode(created);
}

void ProjectPage::addWorkUnit(){
    Project* parent = dynamic_cast<Project*>(selectedNode());
    if(app == nullptr || parent == nullptr) return;
    bool ok = false;
    QString name = QInputDialog::getText(this, "New work unit", "Work unit name:", QLineEdit::Normal, "", &ok).trimmed();
    if(!ok || name.isEmpty()) return;

    QStringList professions;
    professions << "(any)";
    for(const std::string& p : app->getProfessions()) professions << toQ(p);
    QString profession = QInputDialog::getItem(this, "New work unit", "Required profession (pick one or type a new one):",
                                               professions, 0, true, &ok).trimmed();
    if(!ok) return;
    if(profession == "(any)") profession.clear();

    WorkUnit* created = app->addSubcomponent(parent, name.toStdString(), profession.toStdString());
    if(created == nullptr){
        QMessageBox::warning(this, "New work unit", "That name is already used. Names must be unique.");
        return;
    }
    refresh();
    selectNode(created);
}

void ProjectPage::deleteNode(){
    WorkNode* node = selectedNode();
    if(app == nullptr || node == nullptr) return;
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Delete",
        QString("Delete \"%1\" and everything inside it?").arg(toQ(node->getName())));
    if(answer != QMessageBox::Yes) return;
    ui->projectTree->clearSelection();
    app->removeWorkNode(node);
    refresh();
}

void ProjectPage::saveChanges(){
    WorkNode* node = selectedNode();
    if(app == nullptr || node == nullptr) return;

    QString name = ui->nodeNameEdit->text().trimmed();
    if(name.isEmpty() || !app->renameWorkNode(node, name.toStdString())){
        QMessageBox::warning(this, "Save changes", "The name must not be empty or already used by another item.");
        return;
    }
    node->setMaxRetries(ui->maxRetriesSpin->value());
    WorkUnit* unit = dynamic_cast<WorkUnit*>(node);
    if(unit != nullptr) unit->setRequiredProfession(ui->professionCombo->currentText().trimmed().toStdString());
    refresh();
}

void ProjectPage::addPrerequisite(){
    WorkNode* node = selectedNode();
    if(app == nullptr || node == nullptr) return;
    QString name = ui->prerequisiteCombo->currentText();
    if(name.isEmpty()) return;
    if(!app->addPrerequisite(node, name.toStdString())){
        QMessageBox::warning(this, "Add prerequisite",
            "That prerequisite cannot be added. It may already be completed, be one of this item's parents, "
            "or depend on this item.");
        return;
    }
    refresh();
}

void ProjectPage::removePrerequisite(){
    WorkNode* node = selectedNode();
    QList<QListWidgetItem*> picked = ui->prerequisiteList->selectedItems();
    if(app == nullptr || node == nullptr || picked.isEmpty()) return;
    app->removePrerequisite(node, picked.first()->text().toStdString());
    refresh();
}
