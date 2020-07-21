# Rabster--简单的网页呈现
使用到的开源库---感谢
- [Gumbo](https://github.com/google/gumbo-parser) 用于解析Html文件，得到Dom树
- [LibCSS](https://www.netsurf-browser.org/projects/libcss/) 用于解析CSS文件,得到CSS规则并储存
- [Yoga](https://github.com/facebook/yoga) 用于实现Flex布局
- [Gdi+](https://docs.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-gdi-start) 用于最终画面的呈现 
## 当前支持
目前仅支持少量的CSS属性。 

|  CSS属性         | 支持度 |
|  ----           | ----  |
| width           |  ok   |
| height          |  ok   |
| alignContent    |  ok   |
| alignItems      |  ok   |
| alignSelf       |  ok   |
| display         |  ok   |
| flex_wrap       |  ok   |
| flex_grow       |  ok   |
| flex_shrink     |  ok   |
| flex_basis      |  ok   |
| flex_direction  |  ok   |
| justify         |  ok   |
| position        |  ok   |
| margin          |  ok   |
| border          |  ok   |
| padding         |  ok   |
| min-width       |  ok   |
| max-width       |  ok   |
| min-height      |  ok   |
| max-height      |  ok   |
| color           |  ok   | 
| border-color    |  ok   |
| background-color|  ok   |
| transition      | so-so |
| font            | todo  |