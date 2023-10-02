# Assignments

This folder contains the descriptions of assignments to be performed during this course. Assignments are _incremental_.
Each new assignment will be based on the previous ones. For this purpose, you will copy assignments to new folders. How
to do this is described in assignment [`House`](01_House/README.md). Each assignment will be scored. The number of
points assigned to each assignment will appear in the assignment description as well as in the Teams
spreadsheet [zadania](https://ujchmura.sharepoint.com/:x:/r/teams/Section_550032_1/Shared%20Documents/General/zadania.xlsx?d=w2f00faf3a47a4e1e85a4d56be86ed82d&csf=1&web=1&e=fAAxf3)
located in the general channel of the
team [Programowanie grafiki 3D \[ WFAIS.IF-F201.0 23_24Z WYK 1 \]  ](https://teams.microsoft.com/l/team/19%3akawIWvjkm-apQ02mA1Jkt4RKtVhxZcQMsxM5TRvEVc41%40thread.tacv2/conversations?groupId=e025e0b7-c6db-4fc1-9dae-aaff45b35931&tenantId=eb0e26eb-bfbe-47d2-9e90-ebd2426dbceb).

To pass, you have to reach the `DiffuseLightning` assignment, then the final grade will be calculated based on total
number of points. The scale is not yet set as it may depend slightly upon your performance. However, it will be at least
50% of the points for passing, and 95% for the highest grade.

Assignments will have a due date, after which they will no longer be accepted.
You can submit subsequent assignments, but all prior assignments must be included in them.
So you have to do all assignments (up to `DiffuseLighting`) anyway, so you may as well try to submit them on time. The
due date is given in the Teams
spreadsheet [zadania](https://ujchmura.sharepoint.com/:x:/r/teams/Section_550032_1/Shared%20Documents/General/zadania.xlsx?d=w2f00faf3a47a4e1e85a4d56be86ed82d&csf=1&web=1&e=fAAxf3).

In the assignment descriptions, I will omit the arguments of various OpenGL functions. Your task will be to complete
them based on the documentation. Usually, just google the name of the function to get a link
to [OpenGL® 4.5 Reference Pages](https://www.khronos.org/registry/OpenGL-Refpages/gl4/).

## Turning in assignments

You will keep the assignments in your repositories. Unfortunately, we still haven't managed to set up our gitlab.
Therefore, please create **private** repositories on [GitHub](https://github.com/) and connect it
as a remote repository to the local repository where you store your code. You can do it e.g. like this
way:  
First clone my repository as described in the [README.md](../README.md) file.

```shell
git clone https://github.com/pbialas7-Lectures/Graphics3DCode.git
```

Check if everything builds all right, then rename the remote repository

```shell
git remote rename origin origin.lecture
```

and add your GitHub repository as a remote repository

```shell  
git remote add origin <your  GitHub repository>
```

And finally, push the code to your repository

```shell
git push -u origin main
```

In this way, you will be able to pull my changes from my repository and push your changes to your repository. To pull my
changes, you will have to use

```shell
git pull origin.lecture
```

After creating your repository, please give me permission to read/write from/to it (add me to the project). On GitHub
I go by the name `pbialas7`. Please add the URL to the repository to the Teams
sheet [repozytoria](https://ujchmura.sharepoint.com/:x:/r/teams/Section_550032_1/Shared%20Documents/General/repozytoria.xlsx?d=w38fc99320d0f4a2fa9d0bf1f8575ba0c&csf=1&web=1&e=zcoYLf).

The assignments can and are even recommended to be done in pairs. You just need to report it to me in advance and keep
the code in one repository. There is space in the sheet to enter two people for one repository.

## Preparing the assignments

Before starting each assignment, you should copy the directory containing previous assignment. Specifically you should
not modify anything in `src/Assignments/Triangle` folder, but copy it to `src/Assignments/House`  folder. You can do it
using `cp` command (on linux/unix).

```shell
cp -r scr/Assignments/00_Triangle  scr/Assignments/01_House
```

Then you should modify the project name in `src/Assignments/House/CMakeLists.txt` from `Triagle` to `House`.

I have provided a python script that you can use for this:

```shell
python ./scripts/copy_assignment.py 00_Triangle 01_House
```

Use at your own risk!

