![Logo](Logo/logo.png)
# By Vibe Engineers


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

## Logo
Logo was made with this [website](https://madegooddesigns.com/tools/logo-maker/), no AI was involved.

## How to Build the GUI

The GUI is written with Qt 5 (Qt Widgets) and built on Windows with MinGW. The project uses C++11, so use Qt 5.15, the latest Qt 5 release. Everything below was done on Windows.

<details>
<summary><b>1. Install Qt and Qt Creator</b></summary>

1. Make a free account at [qt.io](https://www.qt.io). The installer asks you to log in.
2. Download the **Qt Online Installer** for Windows from the [Qt open source download page](https://www.qt.io/download-open-source) and run it.
3. Log in and accept the open source license terms.
4. On the component selection page, tick the **Archive** checkbox on the right and press **Filter**. Qt 5.15 is an archived release, so it will not show up until you do this.
5. Expand **Qt** and then **Qt 5.15.2**, and tick **MinGW 8.1.0 64-bit**.
6. Expand **Developer and Designer Tools** and tick:
   - **Qt Creator** (normally ticked by default)
   - **MinGW 8.1.0 64-bit** (this is the compiler)
7. Finish the installer. The default install path is `C:\Qt`.

The installer layout changes slightly between versions, but the three things you need are Qt Creator, Qt 5.15.2 for MinGW 64-bit, and the MinGW 64-bit compiler.

</details>

<details>
<summary><b>2. Set up the kit and build</b></summary>

1. Open Qt Creator and go to **File > Open File or Project**, then pick `TIVIDY.pro`.
2. On the **Configure Project** page, tick **Desktop Qt 5.15.2 MinGW 64-bit** and leave all the settings on their defaults.
   - If the kit is missing, go to **Edit > Preferences > Kits** and check that the Qt version and the MinGW compiler were detected.
3. Click the kit selector (the monitor icon at the bottom left) and change the build type from Debug to **Release**.
4. Build with **Ctrl+B** (the hammer icon). Press **Ctrl+R** to run it and check that it works.

Qt Creator makes a build folder named after the kit and the build type. A Debug build goes in `Desktop_Qt_5_15_2_MinGW_64_bit_Debug`, so the Release build goes in `Desktop_Qt_5_15_2_MinGW_64_bit_Release`. Once the build finishes, `TIVIDY.exe` is in the `release` folder inside the Release one. Do not package the Debug build. It is bigger and needs the debug versions of the Qt DLLs.

The stylesheet and images are compiled into the exe through `resources.qrc`, so you do not need to ship them separately.

</details>

<details>
<summary><b>3. Package it with windeployqt</b></summary>

`TIVIDY.exe` on its own only runs on a machine with Qt installed. `windeployqt` copies in the Qt DLLs and plugins it needs.

1. Make a new empty folder (for example `TIVIDY-release`) and copy **only** `TIVIDY.exe` into it. The build's `release` folder also contains `.o`, `moc_` and `ui_` files that you do not want to ship.
2. Open the Start menu and launch **Qt 5.15.2 (MinGW 8.1.0 64-bit)**. This is a command prompt that already has Qt on the PATH.
   - With a normal `cmd` window it will not find `windeployqt` unless you add `C:\Qt\5.15.2\mingw81_64\bin` and `C:\Qt\Tools\mingw810_64\bin` to the PATH first.
3. Go to the folder and run it:
   ```
   cd /d C:\path\to\TIVIDY-release
   windeployqt TIVIDY.exe
   ```
4. The folder now holds the exe, the Qt DLLs and the `platforms` folder, among other files. Zip the whole folder and share the zip.

Test the zip by unzipping it somewhere else and running `TIVIDY.exe`, ideally on a computer that does not have Qt installed.

</details>

<details>
<summary><b>4. Optional: a single exe with Enigma Virtual Box</b></summary>

If you want one file instead of a zip, [Enigma Virtual Box](https://enigmaprotector.com/en/downloads.html) (free) can pack the exe and its DLLs together.

1. Do step 3 first so the folder is complete.
2. Open Enigma Virtual Box.
3. Set **Enter Input File Name** to the `TIVIDY.exe` in your deployed folder.
4. Set **Enter Output File Name** to a different location, for example `TIVIDY-single.exe`. Do not use the same folder as the input.
5. Click **Add** and choose **Add Folder Recursive**, then select the deployed folder. This keeps the `platforms` folder and the other subfolders, which Qt needs.
   - If `TIVIDY.exe` shows up in the file list as well, remove that entry. It is already the input file.
6. Optionally tick **Compress Files** under the file options.
7. Click **Process**.

`TIVIDY-single.exe` runs on its own. It unpacks virtually in memory, so it can start a little slower, and some antivirus programs flag packed exes as false positives. Keep the zip from step 3 as a backup.

</details>
