## 简介

- 实现简单的音乐**推荐**和**搜索**功能  
- 由于用到了Qt，而它对中文较为敏感，故在一些命名方面采用拼音的形式
- 数据结构实验2的作业  

## 下载运行及注意事项

1. ```git clone git@github.com:YJGit/Music-search-and-recommendation-system.git```即可下载全部的内容到本地。
2. 在src/Gui/pages_300里有300首歌，音乐推荐与搜索均在此基础上进行的。  
3. 在exe/下有Gui.exe点击可直接运行，打包好的，**理应在无QT环境的windows平台可以直接运行。**  
4. 由于是分两个阶段完成，故可以看到1，2这样的命名，源码整理得也有些乱。  
5. 推荐功能，**只能根据输入的歌曲名，推荐Top10的歌曲**。查找功能可以随意输入。  
6. **注意数据库的限制，目前仅有300首歌，可以进行拓展。**  

## 效果图  
推荐界面：  
![](http://ouebtut1h.bkt.clouddn.com/rec.PNG)  
推荐功能：  
![](http://ouebtut1h.bkt.clouddn.com/rec_top10.PNG)  
查找界面：  
![](http://ouebtut1h.bkt.clouddn.com/search.PNG)  
查找功能：  
![](http://ouebtut1h.bkt.clouddn.com/sear_no_bound.PNG)  

此外，还有一些小的trick实现，比如歌词的查看，查找结果的翻页等等。  