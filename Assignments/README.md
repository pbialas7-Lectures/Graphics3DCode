# Assignments

This folder contains the descriptions of assignments to be performed during this course. Assignments are _incremental_.
Each new assignment will be based on the previous ones. For this purpose, you will copy assignments to new folders. How
to do this is described in the [Preparing the assignments](#preparing-the-assignments) section below. Each assignment
will be scored. The number of
points assigned to each assignment will appear in the assignment description as well as in the Teams
spreadsheet `assignements`
located in the general channel of the
[course team](https://teams.microsoft.com/l/team/19%3A-rUP1GUAhYdDATjI1djR2MqYPLl8j9igT6vpYDIsdCw1%40thread.tacv2/conversations?groupId=7f734604-dc54-4ac6-9714-3ce4f58e01ba&tenantId=eb0e26eb-bfbe-47d2-9e90-ebd2426dbceb) on Microsoft Teams.

To pass, you have to reach the [`Diffuse`](14_Diffuse/README.md) assignment, then the final grade will be calculated based on total
number of points. The scale is not yet set as it may depend slightly upon your performance. However, it will be at least
50% of the points for passing, and 95% for the highest grade.

Assignments will have a due date, after which they will no longer be accepted.
You can submit subsequent assignments, but all prior assignments must be included in them.
As you have to do all the assignments up to `Diffuse` anyway, you may as well try to submit them on time. The
due date is given in the Teams spreadsheet `zadania` in the course team.

In the assignment descriptions, I will omit the arguments of various OpenGL functions. Your task will be to complete
them based on the documentation. Usually, just search for the name of the function to get a link
to the [OpenGL® 4 Reference Pages](https://registry.khronos.org/OpenGL-Refpages/gl4/).

Please follow the [guidelines](GUIDELINES.md) when working on the assignments. How to find and fix errors in your
code is described in [DEBUGGING.md](DEBUGGING.md).

## List of assignments

| Folder               | Assignment                                              |
|----------------------|---------------------------------------------------------|
| `00_Triangle`        | [Triangle](00_Triangle/README.md)                       |
| `01_House`           | [House](01_House/README.md)                             |
| `02_Colors`          | [Colors](02_Colors/README.md)                           |
| `03_Indices`         | [Indices](03_Indices/README.md)                         |
| `04_Uniforms`        | [Uniforms](04_Uniforms/README.md)                       |
| `05_PVM`             | [Projection - View - Model](05_PVM/README.md)           |
| `06_Pyramid`         | [Pyramid](06_Pyramid/README.md)                         |
| `07_Resize`          | [Resize](07_Resize/README.md)                           |
| `08_Zoom`            | [Zoom](08_Zoom/README.md)                               |
| `09_CameraMovement`  | [Camera Movement](09_CameraMovement/README.md)          |
| `10_Mesh`            | [Mesh](10_Mesh/README.md)                               |
| `11_KdMaterial`      | [KdMaterial](11_KdMaterial/README.md)                   |
| `12_Textures`        | [Textures](12_Textures/README.md)                       |
| `12_z_ComputeShader` | [Compute shader](12_z_ComputeShader/README.md)          |
| `13_OBJReader`       | [Reading Wavefront OBJ files](13_OBJReader/README.md)   |
| `14_Diffuse`         | [Diffuse lighting](14_Diffuse/README.md)                |
| `15_Specular`        | [Specular](15_Specular/README.md)                       |
| `15_z_BlinnPhong_textures` | [Blinn-Phong textures](15_z_BlinnPhong_textures/README.md) |
| `15_za_a_BlinnPhong_normal_map` | [Normal maps](15_za_a_BlinnPhong_normal_map/README.md) |

The `src/Assignments/Debugging` program is the example used in [DEBUGGING.md](DEBUGGING.md).

## Turning in assignments

You will keep the assignments in your repositories. Please create a **private** repository on
[GitHub](https://github.com/) and connect it as a remote repository to the local repository where you store your code.
Create it **empty**, without a README, license or `.gitignore` file, otherwise pushing your code to it will fail.
Pushing to GitHub requires authentication with an [SSH
key](https://docs.github.com/en/authentication/connecting-to-github-with-ssh) or a [personal access
token](https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens);
your GitHub password will not work.

You can connect the repositories e.g. this way. First clone my repository as described in the [README.md](../README.md)
file.

```shell
git clone https://github.com/pbialas7-Lectures/Graphics3DCode.git
```

Check if everything builds all right, then rename the remote repository

```shell
git remote rename origin origin.lecture
```

and add your GitHub repository as a remote repository

```shell
git remote add origin <your GitHub repository>
```

And finally, push the code to your repository

```shell
git push -u origin main
```

In this way, you will be able to pull my changes from my repository and push your changes to your repository. To pull my
changes, you will have to use

```shell
git pull origin.lecture main
```

After creating your repository, please give me permission to read and write to it: in the repository on GitHub go to
*Settings → Collaborators → Add people* and add the user `pbialas7`. Please add the URL to the repository to the Teams
sheet [repozytoria](https://ujchmura.sharepoint.com/:x:/r/teams/Section_628677_1/Shared%20Documents/General/repozytoria.xlsx?d=w8d21fb3ed4354a8985d2e116ea752cb8&csf=1&web=1&e=GDwpxX).

The assignments can and are even recommended to be done in pairs. You just need to report it to me in advance and keep
the code in one repository. There is space in the sheet to enter two people for one repository.

## Preparing the assignments

Before starting each assignment, you should copy the directory containing the previous assignment. Specifically, you
should not modify anything in the `src/Assignments/00_Triangle` folder, but copy it to the `src/Assignments/01_House`
folder. I have provided a Python script that does this:

```shell
python3 ./scripts/copy_assignment.py 00_Triangle 01_House
```

On Windows run it with `py` instead of `python3`. The script copies the folder and changes the project name in
`src/Assignments/01_House/CMakeLists.txt` from `Triangle` to `House`.

You can also do this by hand. On Linux copy the folder with

```shell
cp -r src/Assignments/00_Triangle src/Assignments/01_House
```

and then change the project name in `src/Assignments/01_House/CMakeLists.txt` from `Triangle` to `House`.

Use exactly the folder names given in the [list of assignments](#list-of-assignments): only the folders listed in the
`ASSIGNMENTS` variable in the top `CMakeLists.txt` are built. After copying, configure the project again (in VS Code and
CLion this usually happens automatically), so that the new assignment appears in the list of targets.
