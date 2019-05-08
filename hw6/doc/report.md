### 实现Phong光照模型

#### 场景中绘制一个cube

- 顶点数据

```
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
```

#### Phong光照模型实现原理（Phong shading）

冯氏光照模型(Phong Lighting Model)的主要结构由3个分量组成：环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照。

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/214709_512c4dd4_2165057.png "屏幕截图.png")


- 环境光照(Ambient Lighting)：即使在黑暗的情况下，世界上通常也仍然有一些光亮（月亮、远处的光），所以物体几乎永远不会是完全黑暗的。为了模拟这个，我们会使用一个环境光照常量，它永远会给物体一些颜色。

- 漫反射光照(Diffuse Lighting)：模拟光源对物体的方向性影响(Directional Impact)。它是冯氏光照模型中视觉上最显著的分量。物体的某一部分越是正对着光源，它就会越亮。

- 镜面光照(Specular Lighting)：模拟有光泽物体上面出现的亮点。镜面光照的颜色相比于物体的颜色会更倾向于光的颜色。

##### 环境光照

光通常都不是来自于同一个光源，而是来自于我们周围分散的很多光源，即使它们可能并不是那么显而易见。光的一个属性是，它可以向很多方向发散并反弹，从而能够到达不是非常直接临近的点。所以，光能够在其它的表面上反射，对一个物体产生间接的影响。考虑到这种情况的算法叫做全局照明(Global Illumination)算法，但是这种算法既开销高昂又极其复杂。

由于我们现在对那种又复杂又开销高昂的算法不是很感兴趣，所以我们将会先使用一个简化的全局照明模型，即环境光照。我们使用一个很小的常量（光照）颜色，添加到物体片段的最终颜色中，这样子的话即便场景中没有直接的光源也能看起来存在有一些发散的光。

把环境光照添加到场景里非常简单。我们用光的颜色乘以一个很小的常量环境因子，再乘以物体的颜色，然后将最终结果作为片段的颜色：
```
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
}
```
![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/220332_320e7d44_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/220423_a5b8fd38_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/220443_87a0d101_2165057.png "屏幕截图.png")

应用了环境光照并调节ambient因子的效果如图。

##### 漫反射光照

环境光照本身不能提供最有趣的结果，但是漫反射光照就能开始对物体产生显著的视觉影响了。漫反射光照使物体上与光线方向越接近的片段能从光源处获得更多的亮度。为了能够更好的理解漫反射光照，请看下图：

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/220707_06a30e21_2165057.png "屏幕截图.png")

图左上方有一个光源，它所发出的光线落在物体的一个片段上。我们需要测量这个光线是以什么角度接触到这个片段的。如果光线垂直于物体表面，这束光对物体的影响会最大化（译注：更亮）。为了测量光线和片段的角度，我们使用一个叫做法向量(Normal Vector)的东西，它是垂直于片段表面的一个向量（这里以黄色箭头表示）。这两个向量之间的角度很容易就能够通过点乘计算出来。

- 法向量

法向量是一个垂直于顶点表面的（单位）向量。由于顶点本身并没有表面（它只是空间中一个独立的点），我们利用它周围的顶点来计算出这个顶点的表面。我们能够使用一个小技巧，使用叉乘对立方体所有的顶点计算法向量，但是由于3D立方体不是一个复杂的形状，所以我们可以简单地把法线数据手工添加到顶点数据中。更新后的顶点数据数组可以在这里找到。试着去想象一下，这些法向量真的是垂直于立方体各个平面的表面的（一个立方体由6个平面组成）。

由于我们向顶点数组添加了额外的数据，所以我们应该更新光照的顶点着色器。

```
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
```
还要更新顶点属性指针。

我们只想使用每个顶点的前三个float，并且忽略后三个float，所以我们只需要把步长参数改成float大小的6倍就行了。
```
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```
需要将法向量由顶点着色器传递到片段着色器。

```
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = aNormal;
}
```
接下来，在片段着色器中定义相应的输入变量。

```
in vec3 Normal;
```
- 计算漫反射光照

我们现在对每个顶点都有了法向量，但是我们仍然需要光源的位置向量和片段的位置向量。由于光源的位置是一个静态变量，我们可以简单地在片段着色器中把它声明为uniform：

```
uniform vec3 lightPos;
```
然后在渲染循环中（渲染循环的外面也可以，因为它不会改变）更新uniform。我们使用在前面声明的lightPos向量作为光源位置：

```
lightingShader.setVec3("lightPos", lightPos);

```
最后，我们还需要片段的位置。我们会在世界空间中进行所有的光照计算，因此我们需要一个在世界空间中的顶点位置。我们可以通过把顶点位置属性乘以模型矩阵（不是观察和投影矩阵）来把它变换到世界空间坐标。这个在顶点着色器中很容易完成，所以我们声明一个输出变量，并计算它的世界空间坐标：

```
out vec3 FragPos;  
out vec3 Normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
}
```
最后，在片段着色器中添加相应的输入变量。

在片段着色器中添加光照计算

```
in vec3 FragPos;
```
```
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
```
对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫发射影响。结果值再乘以光的颜色，得到漫反射分量。两个向量之间的角度越大，漫反射分量就会越小：

```
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;
```
如果两个向量之间的角度大于90度，点乘的结果就会变成负数，这样会导致漫反射分量变为负数。为此，我们使用max函数返回两个参数之间较大的参数，从而保证漫反射分量不会变成负数。

现在我们有了环境光分量和漫反射分量，我们把它们相加，然后把结果乘以物体的颜色，来获得片段最后的输出颜色。
```
vec3 result = (ambient + diffuse) * objectColor;
FragColor = vec4(result, 1.0);
```
![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/221907_553c4c33_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/221937_06d43bc4_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/221952_b566fbda_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/222010_f887b051_2165057.png "屏幕截图.png")

应用了环境光照与漫反射光照并调节ambient和diffuse因子的效果如图。


##### 镜面光照

和漫反射光照一样，镜面光照也是依据光的方向向量和物体的法向量来决定的，但是它也依赖于观察方向，例如玩家是从什么方向看着这个片段的。镜面光照是基于光的反射特性。如果我们想象物体表面像一面镜子一样，那么，无论我们从哪里去看那个表面所反射的光，镜面光照都会达到最大化。你可以从下面的图片看到效果：

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/222227_47ff26a8_2165057.png "屏幕截图.png")

我们通过反射法向量周围光的方向来计算反射向量。然后我们计算反射向量和视线方向的角度差，如果夹角越小，那么镜面光的影响就会越大。它的作用效果就是，当我们去看光被物体所反射的那个方向的时候，我们会看到一个高光。

观察向量是镜面光照附加的一个变量，我们可以使用观察者世界空间位置和片段的位置来计算它。之后，我们计算镜面光强度，用它乘以光源的颜色，再将它加上环境光和漫反射分量。

为了得到观察者的世界空间坐标，我们简单地使用摄像机对象的位置坐标代替（它当然就是观察者）。所以我们把另一个uniform添加到片段着色器，把相应的摄像机位置坐标传给片段着色器：

```
uniform vec3 viewPos;
```

```
lightingShader.setVec3("viewPos", camera.Position);
```
现在我们已经获得所有需要的变量，可以计算高光强度了。首先，我们定义一个镜面强度(Specular Intensity)变量，给镜面高光一个中等亮度颜色，让它不要产生过度的影响。

```
float specularStrength = 0.5;
```
下一步，我们计算视线方向向量，和对应的沿着法线轴的反射向量：

```
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
```
需要注意的是我们对lightDir向量进行了取反。reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。第二个参数要求是一个法向量，所以我们提供的是已标准化的norm向量。

剩下要做的是计算镜面分量。

```
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;
```
我们先计算视线方向与反射方向的点乘（并确保它不是负值），然后取它的32次幂。这个32是高光的反光度(Shininess)。一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。在下面的图片里，你会看到不同反光度的视觉效果影响：

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/222430_abba78f6_2165057.png "屏幕截图.png")

把它加到环境光分量和漫反射分量里，再用结果乘以物体的颜色：

```
vec3 result = (ambient + diffuse + specular) * objectColor;
FragColor = vec4(result, 1.0);
```
![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/225644_83087f8d_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/230059_5e38ac06_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/225732_1cdc8391_2165057.png "屏幕截图.png")

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/225823_1065a8eb_2165057.png "屏幕截图.png")

应用了环境光照、镜面光照与漫反射光照并调节ambient、specular和diffuse因子与反光因子的效果如图。

##### 完整着色器代码与GUI

顶点着色器
```
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
}
```
片段着色器
```
#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int Shininess;
uniform float specularStrength;
uniform float ambientStrength;
uniform float diffuseStrength;
void main()
{
    // ambient
    // float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;
    
    // specular
    // float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 
```

```
		ImGui::Text("Press space to stop using mouse.");
		ImGui::Checkbox("Rotate Option", &object_rotate);
		if (object_rotate) {
			ImGui::SliderFloat("RotateX", &rotaX, -360.0f, 360.0f);
			ImGui::SliderFloat("RotateY", &rotaY, -360.0f, 360.0f);
			ImGui::SliderFloat("RotateZ", &rotaZ, -360.0f, 360.0f);

			//ImGui::SliderFloat("Rotate View", &camera_rotate, -360.0f, 360.0f);
			//ImGui::SliderFloat("Radius", &radius, 0.01f, 20.0f);
		}
		ImGui::RadioButton("phong", &light_type, 1);
		ImGui::RadioButton("gouraud", &light_type, 2);

		ImGui::SliderInt("Shininess", &Shininess, 15, 50);
		ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 3.0f);
		ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 3.0f);
		ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 3.0f);

		ImGui::End();

		shader.use();
		shader.setInt("Shininess", Shininess);
		shader.setFloat("specularStrength", specularStrength);
		shader.setFloat("ambientStrength", ambientStrength);
		shader.setFloat("diffuseStrength", diffuseStrength);
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.getPositon());
```

![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/233906_1ad88698_2165057.gif "phong2.gif")

#### Gouraud Shading

Gouraud Shading 与 Phong Shading的不同之处在于 Gouraud Shading 三角形的顶点都有各自的法向量，打光时三个顶点有各自的颜色，接着做双线性内插（bilinear interpolation）来求得颜色，使整个三角形有渐层的颜色变化。Phong shading 三角形的顶点都有各自的法向量，先对三角形整个面作法向量的双线性内插，接着打光来求整个三角形的颜色。


Phong光照模型在顶点着色器中实现：

```
# version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 myColor;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform int Shininess;
uniform float specularStrength;
uniform float ambientStrength;
uniform float diffuseStrength;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 ambient = ambientStrength * lightColor;
    
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = specularStrength * spec * lightColor;  


    myColor = (ambient + diffuse + specular) * objectColor;
}
```
片段着色器

```
#version 330 core
out vec4 FragColor;
in vec3 myColor;

void main()
{
 	FragColor = vec4(myColor, 1.0);
}
```
![输入图片说明](https://images.gitee.com/uploads/images/2019/0508/233251_3457f08f_2165057.gif "gouraud.gif")




