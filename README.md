Git_instructions:包含常用的Git指令以及功能介绍

工作区->暂存区    git add 文件名
暂存区->本地仓库  git commit -m '注释'
查看状态	  git status
查看提交记录      git log 或 git-log       
		  vim ~/.bashrc  添加 alias git-log(别名)='命令'
版本回退          git reset --hard commitID

Head->指向的是当前分支
查看分支   	  git branch   (-vv)查看关联关系
创建分支	  git branch 分支名
创建并切换分支    git checkout -b 分支名
合并分支          git merge 分支名   (提前切换到目标分支中)
删除分支	  git branch -d（D为强制删除） 分支名  


Git远程仓库：Github，Gitee，Gitlab
	 1:创建远程仓库
	 2:创建SSH公钥  ssh-keygen -t rsa
	       查看ssh  cat ~/.ssh/id_rsa.pub
	 3:验证公钥     在Gitee中添加公钥
	 		ssh -T git@gitee.com
	 4:链接Git仓库  git remote add origin SSH
	      查看链接  git remote
                   git remote -v
          删除链接  git remote remove origin
	  推送到远程仓库git push origin master(本地分支):(远程分支)
                   git push -u InterFace InterFace
	      绑定分支  git push --set-upstream origin (本地分支):(远程分支)
	      绑定之后可使用 git push 完成推送
	  git push 出现问题时使用     忽略不相干的历史选项    
	  git push origin master --allow-unrelated-histories

	 5:克隆远程仓库 git clone ssh 名称(文件夹)
	 	   抓取 git fetch origin master//将仓库更新抓取到本地，但不merge
		   拉去 git pull   不需要merge 是抓取的升级
