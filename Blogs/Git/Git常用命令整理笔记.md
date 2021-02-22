### Git简介

Git（读音为/gɪt/）是一个开源的分布式版本控制系统，可以有效、高速地处理从很小到非常大的项目版本管理。分布式相比于集中式的最大区别在于开发者可以提交到本地，每个开发者通过克隆（git clone），在本地机器上拷贝一个完整的Git仓库。

<!--more-->

### 1. Git常用命令表格

|                    命令                     |                        解析                         |
| :-----------------------------------------: | :-------------------------------------------------: |
|                     qwd                     |                 显示当前目录的路径                  |
|                  git init                   | 把当前的目录变成可以管理的git仓库，生成隐藏.git文件 |
|                 git add xxx                 |               把xxx文件添加到暂存区去               |
|             git commit –m 'xxx'             |            提交文件，–m 后面的是提交说明            |
|                 git status                  |                    查看仓库状态                     |
|                git diff xxx                 |              查看xxx文件修改了那些内容              |
|                   git log                   |                    查看历史记录                     |
|              git log --oneline              |               查看历史记录，显示一行                |
|                 git reflog                  |               查看历史记录的版本号id                |
|           git reset --hard HEAD~            |                  回退到上一个版本                   |
|          git reset --hard HEAD~100          |                   回退到100个版本                   |
|                   cat xxx                   |                   查看xxx文件内容                   |
|             git checkout -- xxx             |           把xxx文件在工作区的修改全部撤销           |
|                 git rm xxx                  |                     删除xxx文件                     |
| git remote add origin https://github.com... |                   关联一个远程库                    |
|        git clone https://github.com         |                   从远程库中克隆                    |
|           git push origin master            |            把当前master分支推送到远程库             |
|             git checkout –b dev             |           创建dev分支，并切换到dev分支上            |
|                 git branch                  |                 查看当前所有的分支                  |
|             git checkout 分支名             |                    切换到xxx分支                    |
|                git merge dev                |              在当前的分支上合并dev分支              |
|              git branch –d dev              |                     删除dev分支                     |
|               git branch name               |                      创建分支                       |
|                  git stash                  |   把当前的工作隐藏起来，等以后恢复现场后继续工作    |
|               git stash list                |              查看所有被隐藏的文件列表               |
|               git stash apply               |          恢复被隐藏的文件，但是内容不删除           |
|               git stash drop                |                      删除文件                       |
|                git stash pop                |             恢复文件的同时，也删除文件              |
|                 git remote                  |                  查看远程库的信息                   |
|                git remote –v                |                查看远程库的详细信息                 |
|           git push origin master            |      把master分支推送到远程库对应的远程分支上       |
|              git fetch origin               |                从远程拉取数据到本地                 |

### 2. Git删除指定commit

- **git log**	查看已提交的记录，找到要删除的 **commit**

- 找到此次提交之前的一次提交 **commit-id**

  执行   **git rebase -i xxx(commit-id)**

  按`i`进入编辑模式，将要删除的 **commit** 的`pick`改成`drop`

- 再执行**git log**，查看提交记录，已删除

### 3. 根据 commit-id 查询包含该提交 id 的所有分支

1. 查本地所有分支	git branch --contains commit-id
2. 查远程所有分支    git branch -r --contains commit-id
3. 查本地和远程所有分支    git branch -a --contains commait-id

### 4. git commit 常用命令解析

1. git commit -m '提交信息'

2. git commit -a -m '提交信息'

   -a 可以把还没有执行add命令的修改一起提交

3. git commit --amend

   修改上一次的提交信息；可以将最近的修改追加到上一次的提交上

### 5. Git恢复修改后的文件

1. 只修改了文件，没有任何git操作

   git checkout -- xxx			xxx为文件名

2. 修改了文件，并提交到暂存区（即编辑之后，`git add`但没有`git commit -m`）

   git log --oneline

   git reset HEAD				回退到当前版本

   git checkout -- xxx

3. 修改了文件，并提交了仓库区（即编辑之后，`git add`和`git commit -m`）

   git log --oneline

   git reset HEAD~				回退到上一版本

   git checkout -- xxx