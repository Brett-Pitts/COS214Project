```
 /$$    /$$ /$$ /$$                       /$$$$$$$$                     /$$
| $$   | $$|__/| $$                      | $$_____/                    |__/
| $$   | $$ /$$| $$$$$$$   /$$$$$$       | $$       /$$$$$$$   /$$$$$$  /$$ /$$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$$
|  $$ / $$/| $$| $$__  $$ /$$__  $$      | $$$$$   | $$__  $$ /$$__  $$| $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$ /$$_____/
 \  $$ $$/ | $$| $$  \ $$| $$$$$$$$      | $$__/   | $$  \ $$| $$  \ $$| $$| $$  \ $$| $$$$$$$$| $$$$$$$$| $$  \__/|  $$$$$$
  \  $$$/  | $$| $$  | $$| $$_____/      | $$      | $$  | $$| $$  | $$| $$| $$  | $$| $$_____/| $$_____/| $$       \____  $$
   \  $/   | $$| $$$$$$$/|  $$$$$$$      | $$$$$$$$| $$  | $$|  $$$$$$$| $$| $$  | $$|  $$$$$$$|  $$$$$$$| $$       /$$$$$$$/
    \_/    |__/|_______/  \_______/      |________/|__/  |__/ \____  $$|__/|__/  |__/ \_______/ \_______/|__/      |_______/
                                                              /$$  \ $$
                                                             |  $$$$$$/
                                                              \______/
```



# COS 214 Project: TIVIDY

TIVIDY is a generic workflow management system. A project is a tree of work that moves through a lifecycle, is done by employees, and reports progress up the tree. The system is generic. The scenario is only one use of it.

## Scenario

A large video game company (e.g. Ubisoft) building a live-service game (e.g. Fortnite).

Phases: preparation, creation, marketing, QA testing (external), server preparation, launch.

Failures handled: game cancelled, out of money, deadline missed.

## Team Vibe Engineers

| Member | Student no. |
| --- | --- |
| Jamie King (leader) | u24916031 |
| Louwrens Johansen | u25607414 |
| Brett Pitts | u24682251 |
| Francois Venter | u25555202 |
| Rudolph Botha | u25387023 |

[Current tasks](docs/Current%20Tasks.docx)

## Meeting

[meet.google.com/vyj-iran-psu](https://meet.google.com/vyj-iran-psu)

## Design summary

- **Work tree:** Project (composite) and WorkUnit (leaf) share WorkNode. The tree is the workflow. There is no separate template.
- **Lifecycle:** WaitingForPrerequisites, Ready, Ongoing, Completed, Failed, Escalated, Cancelled.
- **People:** Department and Team form the company tree. Employee is wrapped by profession and competency decorators.
- **Client entry point:** ApplicationInterface (facade).

## Class diagram

![TIVIDY class diagram](docs/UML%20Diagrams/Class%20Diagram/Class%20Diagram.png)

## Patterns (10)

| Pattern | Used for |
| --- | --- |
| Composite (x2) | Work tree; company structure |
| Decorator | Employee profession and competency |
| Factory Method | Creating employees |
| Facade | ApplicationInterface |
| Chain of Responsibility | Finding an eligible employee |
| State | WorkNode lifecycle |
| Observer | Prerequisites and parent/child completion |
| Mediator | Team broadcasts |
| Template Method | prepare, perform, finish |
| Iterator | WorkIterator: next Ready WorkUnit |

## Repository layout

```
README.md
.gitignore
docs/
  COS 214 Project PDF.docx
  COS214_Practical6_TIVIDY.pdf
  Current Tasks.docx
  Task 2 Define the Scenario and System.docx
  Task 4 Design Patterns.docx
  Design Decisions and Revision History/
  Research Material and References/
  UML Diagrams/
    Activity Diagrams/
    Class Diagram/
    Sequence Diagrams/
    State Diagram/
include/
src/
build/
```

## Docs

- [Practical 6 brief](docs/COS214_Practical6_TIVIDY.pdf)
- [Project PDF](docs/COS%20214%20Project%20PDF.docx)
- [Task 2: Define the scenario and system](docs/Task%202%20Define%20the%20Scenario%20and%20System.docx)
- [Task 4: Design patterns](docs/Task%204%20Design%20Patterns.docx)
- [Research](docs/Research%20Material%20and%20References/)
- [Design decisions](docs/Design%20Decisions%20and%20Revision%20History/Design%20Decisions%20and%20Revision%20History.docx)

### UML diagrams

- Activity diagrams:
  [1](docs/UML%20Diagrams/Activity%20Diagrams/Activity%20Diagram1.jpg),
  [2](docs/UML%20Diagrams/Activity%20Diagrams/Activity%20Diagram2.jpg),
  [3](docs/UML%20Diagrams/Activity%20Diagrams/Activity%20Diagram3.jpg)
- [Class diagram](docs/UML%20Diagrams/Class%20Diagram/Class%20Diagram.png)
- Sequence diagrams:
  [1](docs/UML%20Diagrams/Sequence%20Diagrams/Sequence%20Diagram1.jpg),
  [2](docs/UML%20Diagrams/Sequence%20Diagrams/Sequence%20Diagram2.jpg)
- [State diagram](docs/UML%20Diagrams/State%20Diagram/State%20Diagram.jpg)

## Contribution rules

When you work on a task, make a branch for it and merge it into main when you are finished. If your branch needs another branch, wait until that one is merged into main and then pull from main. Whenever you make a design decision, update the Design Decisions and Revision History document.
