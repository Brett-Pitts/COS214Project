#include "companypage.h"
#include "ui_companypage.h"

#include <QInputDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>

#include "ApplicationInterface.h"
#include "Department.h"
#include "Team.h"
#include "EmployeeComponent.h"
#include "EmployeeCompetency.h"

namespace {

	const int NodeRole = Qt::UserRole;

	QString toQ(const std::string& text) {
		return QString::fromStdString(text);
	}

	QString labelFor(CompanyNode* node) {
		QString name = toQ(node->getName());
		Team* team = dynamic_cast<Team*>(node);
		if (team != nullptr) {
			if (name.isEmpty()) name = "(unnamed team)";
			return name;
		}
		return name.isEmpty() ? QString("(unnamed department)") : name;
	}

	CompanyNode* nodeOf(QTreeWidgetItem* item) {
		if (item == nullptr) return nullptr;
		return reinterpret_cast<CompanyNode*>(static_cast<quintptr>(item->data(0, NodeRole).toULongLong()));
	}

	void storeNode(QTreeWidgetItem* item, CompanyNode* node) {
		item->setData(0, NodeRole, static_cast<qulonglong>(reinterpret_cast<quintptr>(node)));
	}

	struct EmployeeInfo {
		QString name;
		QString role;
		QString topCompetency;
	};

	QStringList competencyLevels() {
		return QStringList() << "intern" << "junior" << "senior";
	}

	EmployeeInfo describe(EmployeeComponent* member) {
		EmployeeInfo info;
		info.name = toQ(member->getName());
		info.role = toQ(member->getCore()->getKind());
		QStringList levels = competencyLevels();
		for (int i = levels.size() - 1; i >= 0; i--) {
			if (member->compareCompetency(EmployeeCompetency::getCompetencyLevel(levels[i].toStdString()))) {
				info.topCompetency = levels[i];
				break;
			}
		}
		return info;
	}

}

CompanyPage::CompanyPage(QWidget* parent) : QWidget(parent), ui(new Ui::CompanyPage), app(nullptr) {
	ui->setupUi(this);
	connect(ui->backButton, &QPushButton::clicked, this, &CompanyPage::backRequested);

	connect(ui->companyTree, &QTreeWidget::itemSelectionChanged, this, &CompanyPage::onTreeSelectionChanged);
	connect(ui->addDepartmentButton, &QPushButton::clicked, this, &CompanyPage::addDepartment);
	connect(ui->addTeamButton, &QPushButton::clicked, this, &CompanyPage::addTeam);
	connect(ui->renameButton, &QPushButton::clicked, this, &CompanyPage::renameSelected);
	connect(ui->deleteButton, &QPushButton::clicked, this, &CompanyPage::deleteSelected);

	connect(ui->subItemsList, &QListWidget::itemSelectionChanged, this, &CompanyPage::onSubItemSelectionChanged);
	connect(ui->subItemsList, &QListWidget::itemDoubleClicked, this, &CompanyPage::openSubItem);
	connect(ui->openItemButton, &QPushButton::clicked, this, &CompanyPage::openSubItem);
	connect(ui->addSubDepartmentButton, &QPushButton::clicked, this, &CompanyPage::addSubDepartment);
	connect(ui->addSubTeamButton, &QPushButton::clicked, this, &CompanyPage::addSubTeam);

	connect(ui->employeeTable, &QTableWidget::itemSelectionChanged, this, &CompanyPage::onEmployeeSelectionChanged);
	connect(ui->addEmployeeButton, &QPushButton::clicked, this, &CompanyPage::addEmployee);
	connect(ui->removeEmployeeButton, &QPushButton::clicked, this, &CompanyPage::removeEmployee);
	connect(ui->changeCompetencyButton, &QPushButton::clicked, this, &CompanyPage::changeCompetency);
}

CompanyPage::~CompanyPage() {
	delete ui;
}

void CompanyPage::setApplication(ApplicationInterface* application) {
	app = application;
	refresh();
}

CompanyNode* CompanyPage::selectedNode() const {
	QList<QTreeWidgetItem*> picked = ui->companyTree->selectedItems();
	if (picked.isEmpty()) return nullptr;
	return nodeOf(picked.first());
}

CompanyNode* CompanyPage::shownNode() const {
	CompanyNode* node = selectedNode();
	if (node != nullptr) return node;
	return app != nullptr ? app->getCompany() : nullptr;
}

Team* CompanyPage::shownTeam() const {
	return dynamic_cast<Team*>(shownNode());
}

void CompanyPage::addChildItems(QTreeWidgetItem* parentItem, CompanyNode* node) {
	for (int i = 0;; i++) {
		CompanyNode* child = node->getChild(i);
		if (child == nullptr) break;
		QTreeWidgetItem* item = parentItem != nullptr ? new QTreeWidgetItem(parentItem)
			: new QTreeWidgetItem(ui->companyTree);
		item->setText(0, labelFor(child));
		storeNode(item, child);
		items.insert(child, item);
		addChildItems(item, child);
	}
}

void CompanyPage::refresh() {
	if (app == nullptr) return;

	CompanyNode* keep = selectedNode();

	ui->companyTree->blockSignals(true);
	ui->companyTree->clear();
	items.clear();
	addChildItems(nullptr, app->getCompany());
	ui->companyTree->expandAll();
	QTreeWidgetItem* again = items.value(keep, nullptr);
	if (again != nullptr) ui->companyTree->setCurrentItem(again);
	ui->companyTree->blockSignals(false);

	onTreeSelectionChanged();
}

void CompanyPage::selectNode(CompanyNode* node) {
	QTreeWidgetItem* item = items.value(node, nullptr);
	if (item != nullptr) ui->companyTree->setCurrentItem(item);
}

void CompanyPage::onTreeSelectionChanged() {
	if (app == nullptr) return;

	CompanyNode* selected = selectedNode();
	ui->renameButton->setEnabled(selected != nullptr);
	ui->deleteButton->setEnabled(selected != nullptr);

	Team* team = shownTeam();
	if (team != nullptr) {
		ui->employeeTable->setCurrentCell(-1, -1);
		showTeam(team);
	}
	else {
		showDepartment(shownNode(), selected == nullptr);
	}
}

void CompanyPage::showDepartment(CompanyNode* node, bool isRoot) {
	ui->departmentNameLabel->setText(isRoot ? QString("Company") : labelFor(node));

	ui->subItemsList->blockSignals(true);
	ui->subItemsList->clear();
	int departments = 0;
	int teams = 0;
	for (int i = 0;; i++) {
		CompanyNode* child = node->getChild(i);
		if (child == nullptr) break;
		if (dynamic_cast<Team*>(child) != nullptr) teams++; else departments++;
		QListWidgetItem* entry = new QListWidgetItem(labelFor(child), ui->subItemsList);
		entry->setData(NodeRole, static_cast<qulonglong>(reinterpret_cast<quintptr>(child)));
	}
	ui->subItemsList->blockSignals(false);

	ui->departmentInfoLabel->setText(QString("%1 department(s), %2 team(s)").arg(departments).arg(teams));
	ui->openItemButton->setEnabled(false);
	ui->contentStack->setCurrentWidget(ui->departmentPage);
}

void CompanyPage::onSubItemSelectionChanged() {
	ui->openItemButton->setEnabled(!ui->subItemsList->selectedItems().isEmpty());
}

void CompanyPage::openSubItem() {
	QList<QListWidgetItem*> picked = ui->subItemsList->selectedItems();
	if (picked.isEmpty()) return;
	CompanyNode* node = reinterpret_cast<CompanyNode*>(static_cast<quintptr>(picked.first()->data(NodeRole).toULongLong()));
	selectNode(node);
}

CompanyNode* CompanyPage::containerFor(CompanyNode* node) const {
	if (node != nullptr && dynamic_cast<Team*>(node) == nullptr) return node;
	QTreeWidgetItem* item = items.value(node, nullptr);
	if (item != nullptr && item->parent() != nullptr) return nodeOf(item->parent());
	return app->getCompany();
}

void CompanyPage::createDepartmentIn(CompanyNode* parent) {
	bool ok = false;
	QString name = QInputDialog::getText(this, "New department", "Department name:", QLineEdit::Normal, "", &ok).trimmed();
	if (!ok || name.isEmpty()) return;
	Department* created = app->addDepartment(parent, name.toStdString());
	if (created == nullptr) {
		QMessageBox::warning(this, "New department", "That department could not be added here.");
		return;
	}
	refresh();
	selectNode(created);
}

void CompanyPage::createTeamIn(CompanyNode* parent) {
	bool ok = false;
	QString name = QInputDialog::getText(this, "New team", "Team name:", QLineEdit::Normal, "", &ok).trimmed();
	if (!ok || name.isEmpty()) return;

	QStringList professions;
	for (const std::string& p : app->getProfessions()) professions << toQ(p);
	QString profession = QInputDialog::getItem(this, "New team", "Profession (pick one or type a new one):",
		professions, 0, true, &ok).trimmed();
	if (!ok) return;

	Team* created = app->addTeam(parent, name.toStdString(), profession.toStdString());
	if (created == nullptr) {
		QMessageBox::warning(this, "New team", "That team could not be added here.");
		return;
	}
	refresh();
	selectNode(created);
}

void CompanyPage::addDepartment() {
	if (app == nullptr) return;
	createDepartmentIn(containerFor(selectedNode()));
}

void CompanyPage::addTeam() {
	if (app == nullptr) return;
	createTeamIn(containerFor(selectedNode()));
}

void CompanyPage::addSubDepartment() {
	if (app == nullptr) return;
	createDepartmentIn(containerFor(shownNode()));
}

void CompanyPage::addSubTeam() {
	if (app == nullptr) return;
	createTeamIn(containerFor(shownNode()));
}

void CompanyPage::renameSelected() {
	CompanyNode* node = selectedNode();
	if (app == nullptr || node == nullptr) return;
	bool ok = false;
	QString name = QInputDialog::getText(this, "Rename", "New name:", QLineEdit::Normal,
		toQ(node->getName()), &ok).trimmed();
	if (!ok || name.isEmpty()) return;
	node->setName(name.toStdString());
	refresh();
}

void CompanyPage::deleteSelected() {
	CompanyNode* node = selectedNode();
	if (app == nullptr || node == nullptr) return;
	QMessageBox::StandardButton answer = QMessageBox::question(this, "Delete",
		QString("Delete \"%1\" and everything inside it?").arg(labelFor(node)));
	if (answer != QMessageBox::Yes) return;
	CompanyNode* parent = app->getCompany();
	QTreeWidgetItem* item = items.value(node, nullptr);
	if (item != nullptr && item->parent() != nullptr) parent = nodeOf(item->parent());
	ui->companyTree->clearSelection();
	parent->remove(node);
	delete node;
	refresh();
}

void CompanyPage::showTeam(Team* team) {
	ui->teamNameLabel->setText(labelFor(team));
	QString profession = toQ(team->getProfession());
	if (profession.isEmpty()) {
		profession = "any";
	}
	ui->teamInfoLabel->setText(QString("Profession: %1\n%2 member(s)").arg(profession).arg(team->getMemberCount()));

	int keepRow = ui->employeeTable->currentRow();
	ui->employeeTable->blockSignals(true);
	ui->employeeTable->clearContents();
	ui->employeeTable->setRowCount(team->getMemberCount());
	for (int row = 0; row < team->getMemberCount(); row++) {
		EmployeeInfo info = describe(team->getMember(row));
		ui->employeeTable->setItem(row, 0, new QTableWidgetItem(info.name));
		ui->employeeTable->setItem(row, 1, new QTableWidgetItem(info.role));
		QString competency = info.topCompetency;
		if (competency.isEmpty()) {
			competency = "-";
		}
		ui->employeeTable->setItem(row, 2, new QTableWidgetItem(competency));
	}
	if (keepRow >= 0 && keepRow < team->getMemberCount()) ui->employeeTable->selectRow(keepRow);
	ui->employeeTable->blockSignals(false);

	ui->contentStack->setCurrentWidget(ui->teamPage);
	onEmployeeSelectionChanged();
}

void CompanyPage::onEmployeeSelectionChanged() {
	Team* team = shownTeam();
	int row = ui->employeeTable->currentRow();
	bool picked = team != nullptr && row >= 0 && row < team->getMemberCount()
		&& !ui->employeeTable->selectedItems().isEmpty();

	ui->removeEmployeeButton->setEnabled(picked);
	ui->changeCompetencyButton->setEnabled(picked);
	ui->detailsBox->setEnabled(picked);

	if (!picked) {
		ui->employeeNameLabel->setText("-");
		ui->employeeRoleLabel->setText("-");
		ui->competencyValueLabel->setText("-");
		return;
	}

	EmployeeInfo info = describe(team->getMember(row));
	ui->employeeNameLabel->setText(info.name);
	ui->employeeRoleLabel->setText(info.role);
	ui->competencyValueLabel->setText("-");
	if (!info.topCompetency.isEmpty()) ui->competencyValueLabel->setText(info.topCompetency);
}

void CompanyPage::addEmployee() {
	Team* team = shownTeam();
	if (app == nullptr || team == nullptr) return;

	bool ok = false;
	QString name = QInputDialog::getText(this, "Add employee", "Name:", QLineEdit::Normal, "", &ok).trimmed();
	if (!ok || name.isEmpty()) return;

	QString profession = toQ(team->getProfession());
	if (profession.isEmpty()) {
		QStringList professions;
		for (const std::string& p : app->getProfessions()) professions << toQ(p);
		profession = QInputDialog::getItem(this, "Add employee", "Profession (pick one or type a new one):",
			professions, 0, true, &ok).trimmed();
		if (!ok) return;
	}

	QString competency = QInputDialog::getItem(this, "Add employee", "Competency:", competencyLevels(), 0, false, &ok);
	if (!ok) return;
	QString role = QInputDialog::getItem(this, "Add employee", "Role:", QStringList() << "Employee" << "Manager", 0, false, &ok);
	if (!ok) return;

	if (!app->addEmployee(team, name.toStdString(), profession.toStdString(), competency.toStdString(), role == "Manager")) {
		QMessageBox::warning(this, "Add employee", "That employee could not be added to this team.");
		return;
	}
	showTeam(team);
	ui->employeeTable->selectRow(team->getMemberCount() - 1);
}

void CompanyPage::removeEmployee() {
	Team* team = shownTeam();
	int row = ui->employeeTable->currentRow();
	if (app == nullptr || team == nullptr || row < 0) return;
	QMessageBox::StandardButton answer = QMessageBox::question(this, "Remove employee", "Remove this employee from the team?");
	if (answer != QMessageBox::Yes) return;
	team->removeMember(row);
	ui->employeeTable->clearSelection();
	showTeam(team);
}

void CompanyPage::changeCompetency() {
	Team* team = shownTeam();
	int row = ui->employeeTable->currentRow();
	if (app == nullptr || team == nullptr || row < 0) return;
	bool ok = false;
	QString competency = QInputDialog::getItem(this, "Change competency", "Competency:", competencyLevels(), 0, false, &ok);
	if (!ok) return;
	if (!app->setCompetency(team, row, competency.toStdString())) {
		QMessageBox::warning(this, "Change competency", "That competency could not be set.");
		return;
	}
	showTeam(team);
	ui->employeeTable->selectRow(row);
}
