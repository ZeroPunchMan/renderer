# 软件渲染器 
这是一个软件非常简单的软件渲染器,主要是将Mesh(仅支持三角形)从世界空间变换到屏幕空间(仅实现了透视投影),再进行光栅化完成渲染.

## 使用方法

__renderer.exe -f FBX_FILE -t TEXTURE__

__FBX_FILE:指定要渲染的FBX文件__

__TEXTURE:指定使用的纹理__

__目前仅测试了项目自带的Teapot等FBX文件__

使用WASD以及鼠标左右键拖拽来调整镜头，滚轮可以调整FOV，操作手感不太好.

## 效果演示

![](https://raw.githubusercontent.com/knightlyj/renderer/master/docs/img/teapot.png)

## 动机
在学习了一些基本的实时渲染知识之后,总感觉有些地方想的不够清楚,在网上看到这类建议,所以做了这个项目.
由于很多知识点都是临时抱佛脚,软件质量可想而知非常堪忧,但这个开发过程中确实学到了非常多的知识,其中自创许多"定理",并进行了证明.
如果有同学也在做这方面软件,有问题的话可以互相交流一下(发邮件即可).

## 实现
主要逻辑非常简单，用direct2D创建一个ID2D1Bitmap，将一个个三角形渲染到这个bitmap上，再由direct2D渲染这个bitmap.

其中核心问题是如何将三角形渲染到bitmap，代码流程如下,看一下注释就能知道流程了.
```
//背面剔除
if (BackFaceCull(&v0, &v1, &v2))
	return;

//变换到摄像机坐标系
v0.homoCoord = this->WorldToCamera(v0.homoCoord);
v1.homoCoord = this->WorldToCamera(v1.homoCoord);
v2.homoCoord = this->WorldToCamera(v2.homoCoord);

//frustum剔除
if (FrustumCull(&v0, &v1, &v2))
	return;

//透视矩阵变换
v0.homoCoord = this->PerspectiveProj(v0.homoCoord);
v1.homoCoord = this->PerspectiveProj(v1.homoCoord);
v2.homoCoord = this->PerspectiveProj(v2.homoCoord);

//齐次坐标裁剪
vector<Vertex> unclip;
unclip.push_back(v0);
unclip.push_back(v1);
unclip.push_back(v2);

vector<Vertex> clipped;
this->HomoClip(&unclip, &clipped);
if (clipped.size() == 0) {
	return;
}


vector<Vertex>::iterator it;
for (it = clipped.begin(); it != clipped.end(); it++) {
	//透视除法
	it->homoCoord.pos /= it->homoCoord.w;
}

//三角化
vector<int> triangles;
this->Triangulate(&clipped, &triangles);


//视口坐标渲染三角形
int count = triangles.size() / 3;
for (int i = 0; i < count; i++) {
	int a, b, c;
	a = triangles[i * 3];
	b = triangles[i * 3 + 1];
	c = triangles[i * 3 + 2];
	pRenderTexture->DrawTriangle(&clipped[a], &clipped[b], &clipped[c]);
}
```

