
#include "taskpage.h"
#include "ui_taskpage.h"

#include <QInputDialog>
#include <QListWidget>
#include <QRegularExpression>
#include <QStringList>
#include <QTableWidgetItem>

#include "ApplicationInterface.h"
#include "Team.h"
#include "WorkUnit.h"

namespace {

	QString toQ(const std::string& text) {
		return QString::fromStdString(text);
	}

	QString prettyState(const std::string& state) {
		QString text = toQ(state);
		text.replace(QRegularExpression("([a-z])([A-Z])"), "\\1 \\2");
		return text;
	}

	bool isFinished(const std::string& state) {
		return state == "Completed" || state == "Cancelled" || state == "Failed";
	}

	QStringList stateNames() {
		return QStringList() << "WaitingForPrerequisites" << "Ready" << "Ongoing" << "Escalated"
			<< "Completed" << "Cancelled" << "Failed";
	}

}

TaskPage::TaskPage(QWidget* parent)
	: QWidget(parent), ui(new Ui::TaskPage), app(nullptr), teamsFor(nullptr) {
	ui->setupUi(this);
	ui->assignmentTable->setColumnWidth(0, 220);
	ui->assignmentTable->setColumnWidth(1, 200);

	connect(ui->backButton, &QPushButton::clicked,
		this, &TaskPage::backRequested);

	connect(ui->workUnitList, &QListWidget::itemSelectionChanged,
		this, &TaskPage::updateSelection);
	connect(ui->assignmentTable, &QTableWidget::itemSelectionChanged,
		this, &TaskPage::updateSelection);

	connect(ui->assignButton, &QPushButton::clicked, this, &TaskPage::assign);
	connect(ui->unassignButton, &QPushButton::clicked, this, &TaskPage::unassign);
	connect(ui->stepButton, &QPushButton::clicked, this, &TaskPage::stepSelected);
	connect(ui->cancelWorkButton, &QPushButton::clicked, this, &TaskPage::cancelSelected);
	connect(ui->forceFailButton, &QPushButton::clicked, this, &TaskPage::forceFailSelected);
	connect(ui->forceStateButton, &QPushButton::clicked, this, &TaskPage::forceStateSelected);
	ui->unassignButton->setToolTip("Removes the assignment from the list. Work the team already did is not undone.");

	updateSelection();
}

TaskPage::~TaskPage() {
	delete ui;
}

void TaskPage::setApplication(ApplicationInterface* application) {
	app = application;
	refresh();
}

void TaskPage::reset() {
	ui->workUnitList->blockSignals(true);
	ui->workUnitList->clear();
	ui->workUnitList->blockSignals(false);
	units.clear();
	teamChoices.clear();
	teamsFor = nullptr;
	ui->resultLabel->clear();
}

WorkUnit* TaskPage::selectedUnit() const {
	int row = ui->workUnitList->currentRow();
	if (row < 0 || row >= static_cast<int>(units.size()) || ui->workUnitList->selectedItems().isEmpty())
		return nullptr;
	return units[row];
}

void TaskPage::refresh() {
	if (app == nullptr)
		return;

	QString keepName;
	WorkUnit* keep = selectedUnit();
	if (keep != nullptr)
		keepName = toQ(keep->getName());

	teamsFor = nullptr;
	ui->workUnitList->blockSignals(true);
	ui->workUnitList->clear();
	units = app->getWorkUnits();
	for (size_t i = 0; i < units.size(); i++) {
		QString text = QString("%1  [%2]").arg(toQ(units[i]->getName()), prettyState(units[i]->getStateName()));
		ui->workUnitList->addItem(text);
		if (!keepName.isEmpty() && toQ(units[i]->getName()) == keepName)
			ui->workUnitList->setCurrentRow(static_cast<int>(i));
	}
	ui->workUnitList->blockSignals(false);

	ui->assignmentTable->blockSignals(true);
	ui->assignmentTable->clearContents();
	const std::map<std::string, std::string>& assignments = app->getAssignments();
	ui->assignmentTable->setRowCount(static_cast<int>(assignments.size()));
	int row = 0;
	for (std::map<std::string, std::string>::const_iterator it = assignments.begin(); it != assignments.end(); ++it) {
		ui->assignmentTable->setItem(row, 0, new QTableWidgetItem(toQ(it->first)));
		ui->assignmentTable->setItem(row, 1, new QTableWidgetItem(toQ(it->second)));
		QString stateText = "-";
		for (size_t i = 0; i < units.size(); i++) {
			if (units[i]->getName() == it->first) {
				stateText = prettyState(units[i]->getStateName());
			}
		}
		ui->assignmentTable->setItem(row, 2, new QTableWidgetItem(stateText));
		row++;
	}
	ui->assignmentTable->blockSignals(false);

	updateSelection();
}

void TaskPage::updateSelection() {
	WorkUnit* unit = selectedUnit();
	bool unitPicked = unit != nullptr;

	ui->selectedWorkUnitLabel->setText(unitPicked ? toQ(unit->getName())
		: "None selected");

	if (unit != teamsFor) {
		teamsFor = unit;
		ui->teamCombo->clear();
		teamChoices.clear();
		if (app != nullptr && unitPicked) {
			std::cout << "Required: " << unit->getRequiredProfession() << '\n';
			teamChoices = app->getTeams(unit->getRequiredProfession());
			for (size_t i = 0; i < teamChoices.size(); i++) {
				QString label = toQ(teamChoices[i]->getName());
				if (label.isEmpty())
					label = "(unnamed team)";
				ui->teamCombo->addItem(label);
			}
		}
	}

	ui->assignButton->setEnabled(unitPicked && !teamChoices.empty());

	bool canStep = false;
	bool canCancel = false;
	if (unitPicked && app != nullptr && !isFinished(unit->getStateName())) {
		canCancel = true;
		canStep = app->getAssignments().count(unit->getName()) > 0;
	}
	ui->stepButton->setEnabled(canStep);
	ui->forceFailButton->setEnabled(canStep);
	ui->forceStateButton->setEnabled(unitPicked);
	ui->cancelWorkButton->setEnabled(canCancel);
	ui->unassignButton->setEnabled(!ui->assignmentTable->selectedItems().isEmpty());
}

void TaskPage::assign() {
	WorkUnit* unit = selectedUnit();
	int teamIndex = ui->teamCombo->currentIndex();
	if (app == nullptr || unit == nullptr || teamIndex < 0 || teamIndex >= static_cast<int>(teamChoices.size()))
		return;

	ui->resultLabel->setText(toQ(app->assignOnly(unit, teamChoices[teamIndex])));
	refresh();
}

void TaskPage::stepSelected() {
	WorkUnit* unit = selectedUnit();
	if (app == nullptr || unit == nullptr)
		return;

	ui->resultLabel->setText(toQ(app->stepWork(unit)));
	refresh();
}

void TaskPage::cancelSelected() {
	WorkUnit* unit = selectedUnit();
	if (app == nullptr || unit == nullptr)
		return;

	ui->resultLabel->setText(toQ(app->cancelWork(unit)));
	refresh();
}

void TaskPage::forceFailSelected() {
	WorkUnit* unit = selectedUnit();
	if (app == nullptr || unit == nullptr)
		return;

	ui->resultLabel->setText(toQ(app->failWork(unit)));
	refresh();
}

void TaskPage::forceStateSelected() {
	WorkUnit* unit = selectedUnit();
	if (app == nullptr || unit == nullptr)
		return;

	QStringList names = stateNames();
	QStringList shown;
	for (int i = 0; i < names.size(); i++)
		shown << prettyState(names[i].toStdString());

	int current = shown.indexOf(prettyState(unit->getStateName()));
	if (current < 0)
		current = 0;

	bool ok = false;
	QString picked = QInputDialog::getItem(this, "Force state", "Set state to:", shown, current, false, &ok);
	if (!ok)
		return;

	int index = shown.indexOf(picked);
	if (index < 0)
		return;

	ui->resultLabel->setText(toQ(app->forceState(unit, names[index].toStdString())));
	refresh();
}

void TaskPage::unassign() {
	QList<QTableWidgetItem*> picked = ui->assignmentTable->selectedItems();
	if (app == nullptr || picked.isEmpty())
		return;
	QTableWidgetItem* nameItem = ui->assignmentTable->item(picked.first()->row(), 0);
	if (nameItem == nullptr)
		return;
	app->unassign(nameItem->text().toStdString());
	refresh();
}
