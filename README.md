![Logo](Logo/logo.png)
# By Vibe Engineers


TIVIDY is a generic workflow management system. Work is organised as a tree of projects and tasks, each moving through a clear lifecycle, assigned to skilled employees, and reporting its progress up the tree. Nothing in the system is tied to one industry, so the scenarios below are just two examples of what it can run.

## Scenarios

### Scenario A: Game studio

A large video game company (e.g. Ubisoft) building a live-service game (e.g. Fortnite).

Phases: preparation, creation, marketing, QA testing (external), server preparation, launch.

Failures handled: game cancelled, out of money, deadline missed.

### Scenario B: Construction company

A construction company building a house.

Phases: design, structure, services, inspection, handover.

Teams: architects, engineers, bricklayers, plumbers, electricians, a site manager and a municipal inspector.

## Team Vibe Engineers

| Member | Student no. |
| --- | --- |
| Jamie King (leader) | u24916031 |
| Louwrens Johansen | u25607414 |
| Brett Pitts | u24682251 |
| Francois Venter | u25555202 |
| Rudolph Botha | u25387023 |

## Design summary

- **Work tree:** Project (composite) and WorkUnit (leaf) share WorkNode. The tree is the workflow. There is no separate template.
- **Lifecycle:** WaitingForPrerequisites, Ready, Ongoing, Completed, Failed, Escalated, Cancelled.
- **People:** Department and Team form the company tree. Employee is wrapped by profession and competency decorators and is created through an employee builder.
- **Saving:** Memento captures and restores state, written to the `saves` folder.
- **Client entry point:** ApplicationInterface (facade).

## Class diagram

![TIVIDY class diagram](diagrams/UML%20Diagrams/Class%20Diagram/Class%20Diagram.png)

## Patterns (12)

| Pattern | Used for |
| --- | --- |
| Composite (x2) | WorkNode, Project and WorkUnit form the work tree; Department, Team and CompanyNode form the company structure |
| Decorator | EmployeeProfession and EmployeeCompetency wrap an Employee to add a profession and a competency level |
| Builder | EmployeeBuilder, ConcreteEmployeeBuilder and EmployeeDirector assemble an employee from optional parts |
| Prototype | clone() copies work nodes, employees, decorator chains and teams without rebuilding them |
| Facade | ApplicationInterface gives the GUI one entry point, including creating Scenario A and B |
| Chain of Responsibility | Team members are linked by successor; handleWork() passes a work unit along until an eligible employee takes it |
| State | WorkNode delegates its lifecycle to WorkState and its concrete states (Ready, Ongoing, Escalated and so on) |
| Mediator (x2) | WorkMediator announces completion between work nodes; Team mediates broadcasts between its employees |
| Template Method | doWork() runs prepare, perform and finish, and each decorator adds its own part |
| Iterator | WorkIterator walks the work tree to find the next Ready WorkUnit; TeamIterator walks the company tree for Teams |
| Memento | Memento stores a copy of the company or workflow, which FileSystem saves to and loads from the saves folder |


## Docs

The Doxygen documentation for the project can be viewed here: [https://brett-pitts.github.io/COS214Project/](https://brett-pitts.github.io/COS214Project/)

### Practical 6

- [Practical 6 instructions](diagrams/Project%20Instructions/Practical%206.pdf)
- [Practical 6 full submission](diagrams/COS%20214%20Prac%206%20PDF.pdf)
- [Task 2: Define the scenario and system](diagrams/Practical%206%20Tasks/Task%202%20Define%20the%20Scenario%20and%20System.docx)
- [Task 4: Design patterns](diagrams/Practical%206%20Tasks/Task%204%20Design%20Patterns.docx)
- [Research material](diagrams/Research%20Material%20and%20References/Research%20Material%20and%20References.docx)

### Final submission

- [Final submission instructions](diagrams/Project%20Instructions/Final%20Project.pdf)
- [Final report](diagrams/TIVIDY%20Final%20Report.docx)
- [Design decisions and revision history](diagrams/Design%20Decisions%20and%20Revision%20History/)

### UML diagrams

- Activity diagrams:
  [1](diagrams/UML%20Diagrams/Activity%20Diagrams/Activity%20Diagram%201.jpg),
  [2](diagrams/UML%20Diagrams/Activity%20Diagrams/Activity%20Diagram%202.png),
  [3](diagrams/UML%20Diagrams/Activity%20Diagrams/Activity%20Diagram%203.jpg)
- [Class diagram](diagrams/UML%20Diagrams/Class%20Diagram/Class%20Diagram.png)
- Sequence diagrams:
  [1](diagrams/UML%20Diagrams/Sequence%20Diagrams/Sequence%20Diagram%201.png),
  [2](diagrams/UML%20Diagrams/Sequence%20Diagrams/Sequence%20Diagram%202.png)
- [State diagram](diagrams/UML%20Diagrams/State%20Diagram/State%20Diagram.png)

## Contribution rules

When you work on a task, make a branch for it and merge it into main when you are finished. If your branch needs another branch, wait until that one is merged into main and then pull from main. Whenever you make a design decision, add a note to the [Design Decisions and Revision History](diagrams/Design%20Decisions%20and%20Revision%20History/) folder.

## Logo
Logo was made with this [website](https://madegooddesigns.com/tools/logo-maker/), no AI was involved.

## How to Build the application

**Windows users:** all the commands below must be run inside WSL, and Docker Desktop must have WSL integration enabled.

Clone the project

```bash
git clone https://github.com/Brett-Pitts/COS214Project
```

Go to the project directory

```bash
cd COS214Project
```

Build Docker Image
```bash
docker build -t tividy .
```

Run Docker
```bash
docker run -it \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
  tividy /bin/bash
```

Compile

```bash
qmake GUI/TIVIDY.pro
make -j$(nproc)
```

Run

```bash
./GUI/TIVIDY
```


Run ScenarioA

```bash
./GUI/TIVIDY ScenarioA
```

Run ScenarioB

```bash
./GUI/TIVIDY ScenarioB
```


Run Tests
```bash
make -C src test
make -C src clean
```

## How to generate the Doxygen documentation

From the project root, run

```bash
doxygen Doxyfile
```

The generated files are written to the `docs` folder, and `docs/index.html` is the starting page.
