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

**Team Vibe Engineers**

# TIVIDY: Workflow Management System

COS 214 Project, Team **Vibe Engineers**. Language: C++11.

TIVIDY is a generic workflow management system. A project is a tree of work that moves through a lifecycle, is done by employees, and reports progress up the tree. The system is generic. The scenario is only one use of it.

## Scenario

A large video game company (e.g. Ubisoft) building a live-service game (e.g. Fortnite).

Phases: preparation, creation, marketing, QA testing (external), server preparation, launch.

Failures handled: game cancelled, out of money, deadline missed.

## Team

| Member | Student no. | Role |
| --- | --- | --- |
| Jamie King (leader) | u24916031 | Design patterns |
| Louwrens Johansen | u25607414 | Class diagram |
| Brett Pitts | u24682251 | Activity diagrams |
| Francois Venter | u25555202 | Research |
| Rudolph Botha | u25387023 | Runtime diagrams |

## Meeting

COS 214 Project Meeting (booked by Francois Venter)

- **When:** Thu 8 Oct 2026, 12:00 to 12:20 (South Africa time, GMT+2)
- **Group name:** Vibe Engineers
- **Link:** [meet.google.com/vyj-iran-psu](https://meet.google.com/vyj-iran-psu)

## Design at a glance

- **Work tree:** `Project` (composite) and `WorkUnit` (leaf) share `WorkNode`. The tree is the workflow. There is no separate template.
- **Lifecycle:** WaitingForPrerequisites, Ready, Ongoing, Completed, Failed, Escalated, Cancelled.
- **People:** `Department` and `Team` form the company tree. `Employee` is wrapped by profession and competency decorators.
- **Client entry point:** `ApplicationInterface` (facade).

## Class diagram

![TIVIDY class diagram](docs/UML%20Diagrams/Class%20Diagram/Class%20Diagram.png)

## Patterns (10)

| Pattern | Used for |
| --- | --- |
| Composite (x2) | Work tree; company structure |
| Decorator | Employee profession and competency |
| Factory Method | Creating employees |
| Facade | `ApplicationInterface` |
| Chain of Responsibility | Finding an eligible employee |
| State | `WorkNode` lifecycle |
| Observer | Prerequisites and parent/child completion |
| Mediator | `Team` broadcasts |
| Template Method | prepare, perform, finish |
| Iterator | `WorkIterator`: next Ready `WorkUnit` |

## Repository layout

```
README.md
docs/
  Design Decisions and Revision History/   Task 7
  Research Material and References/        Task 1
  UML Diagrams/
    Activity Diagrams/                     Activity Diagram1.jpg to Activity Diagram3.jpg
    Class Diagram/                         Class Diagram.png and .drawio
    Sequence Diagrams/                     Sequence Diagram1.jpg, Sequence Diagram2.jpg
    State Diagram/                         State Diagram.jpg
src/                                       C++ source (later phases)
```

## Docs

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

- Work on a branch, open a pull request, one reviewer.
- Commit messages say what changed and why.
- Update the Design Decisions and Revision History document whenever a diagram changes.
