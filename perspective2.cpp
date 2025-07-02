#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iomanip>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <shader.h>
#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

template <typename T>
void outMatrixFormatted(T matrix) {
	int col = matrix.length(); 
	int row = matrix[0].length();
	cout << fixed << setprecision(2);
	for (int j = 0; j < col; j++) {
		cout << "[";
		for (int i = 0; i < row; i++) {
			cout << (float)matrix[i][j];
			if (i != row - 1) cout << ", ";
		}
		cout << "]\n";
	}
	cout << "[.]\n";
	return;
}

template <typename T> 
void outVectorFormatted(T V) {
	int length = V.length(); 
	cout << fixed << setprecision(6);
	for (int i = 0; i < length; i++) cout << "[" << (float)V[i] << "]\n";
	cout << "[.]\n";
	return;
}

float mixValue = 0;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Shader ourShader("./shader/vertexShader.txt", "./shader/fragmentShader.txt");

    float vertices[] = {
        -1.0f, 1.0f, 0.0f,      1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,      0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };

    // float vertices[] = {
    //     // x, y, z, w
    //     -1.0f, 1.0f, 0.5f, 1.5f,        1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    //     1.0f, 1.0f,  3.5f, 2.5f,        0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    //     1.0f, -1.0f, 0.5f, 1.5f,        0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    //     -1.0f, -1.0f, 0.5f, 1.5f,       1.0f, 1.0f, 0.0f,   0.0f, 1.0f
    // };

    /*
    
    khi sử dụng x, y, z, w đưa trực tiếp cho openGL openGL sẽ chuẩn hóa nó bằng cách chia x, y,z cho w 
    chỉ có các điểm ảnh có tọa độ nằm trong khoảng [-1, 1] openGL mới vẽ nó lên màn hình 

    sau khi openGL chia tọa độ xong openGL sẽ sử dụng tọa độ x, y, z ánh xạ nó lên màn hình 1 cách hợp lý nhất 
    với các tọa độ của x hoặc y hoặc z nằm ngoài đoạn -1, 1 openGL sẽ không vẽ các điểm ảnh có tọa độ đó 

    với điểm ở trên chúng ta có thể thấy rằng 3.5/2.5 > 1-> openGL sẽ không vẽ các điểm nằm ở đó 
    bản chất khi chung ta vẽ tam giác chúng ta đang vẽ rất nhiều điểm ảnh chứ không phải 1 điểm ảnh 
    GPU sẽ xử lý các điểm ảnh riêng biệt từ đó kết luận từ đó có hay không điểm ảnh đó được vẽ lên màn hình 

    bản chất w không phải là độ sâu nó chỉ là tỉ lệ mà chúng ta mong muốn để đưa vào tạo độ chuẩn hóa đồng thời 
    tạo ra hiệu ứng xa gần perspective đối với các vật thể độ sâu ở đây bản chất vẫn là tham số z chúng ta có thể hình dung như 
    
    openGL sẽ không vẽ các vật ở quá xa hay gần hơn vùng nhìn thấy tức là khi z quá  to hay quá nhỏ hàm cho tỉ lệ z/w không nằm trong đoạn [-1, 1]
    openGL sẽ coi như không thấy nó 

    tóm lại khi dùng perspective các tọa độ của chúng ta có thể để tự do thoải mái -100, 100 cũng được tại vì khi nhân qua ma trận đó các điểm 
    đó sẽ được chuẩn hóa dựa trên perspective mà chúng ta đã set 

    cái thứ 2 đó là tại sao lại phải là cả 3 thông số x, y, z chia cho w mà không phải chỉ là mỗi z thôi đó là lúc bạn chưa hiểu 
    còn bây giờ khi bạn hiêu rồi thì bản chất đó là khoảng các từ 1 điểm đến camera trong openGL nó là căn x^2 + y^2 + z^ 2
    tức là khi x, y, z càng đó thì điểm đó càng xa camera chúng ta cần phải cái đó chứ không phải chỉ là z to mà là độ sâu lớn 
    bản chất độ sâu là khoảng các từ điểm đó đến camera nên chúng ta cần chia cho cả 3 tham số đó là x,y,z 


    tóm lại : 
        GPU : chỉ hoạt động trên shader các tọa độ được ánh xạ qua 4 tham số (x, y, z, w) sau đó được chuẩn hóa 
                - nếu nằm trong vùng nhìn thấy vì vẽ điểm ảnh đó 
                - nếu không nằm trong vùng -1, 1 thì không vẽ điểm ảnh đó 
            
        - Khi sử dụng perspective thì tọa độ của chúng ta có thể thoải mái lựa chọn không chỉ nằm trong đoạn -1, 1 nữa
        - Vì khi nhân qua ma trận các tọa độ sẽ được chuẩn hóa lại cho openGL vẽ ngoài ra khi sử dụng perspective 
        - Các tọa độ đó còn được chuẩn hóa 1 cách để tạo ra hiệu ứng xa gần rất hay 

        - Chúng ta có thể nhìn thấy chúng khi nhìn vào 2 vertices[] ở bên dưới chúng đều render ra 1 hình ảnh như nhau mặc dù tạo độ 
        - của chúng ban đầu là khác nhau do chúng ta đã sử dụng perspective và đương nhiên cái w khi sử dụng perspective chúng ta 
        - nên để nó là 1 

        - Cái chung ta đang thắc mắc đó là tại sao điểm có độ cao khác nhau openGL không chỉ đơn giản là

            - chúng ta vừa ngộ nhận ở chỗ này 
                - thuật ra khi ánh xạ nên mặt phẳng 2D openGL khi xong nước NDC openGL không quan tâm đến tọa độ của z nửa mà chỉ 
                - dùng tọa độ của x, y thông qua NDC để ánh xạ thôi 

            - chúng ta vừa mới sai lầm do chúng ta sử dụng perspective tức là chúng ta vừa thay đổi x, y sao cho thỏa mãn perspective 
            - ok sai lầm này là do chúng ta ngộ nhận còn openGL thật sự không sử dụng x,y nhé 

            - x, y được thay đổi là do hàm glm::perspective mà chúng ta đã cài đặt 
                - chỉ quan tâm đến ham số x, y mà ánh xạ nó xuống mặt phẳng thôi // sai lầm nhé 
                - openGL còn quan tâm đến tham số z nữa từ đó ánh xạ nó xuống 1 cách rất hợp lý 
                - chúng ta cần tìm ra bí mật này trong tối nay 
                - chúng ta có thể thử vẽ 2 tam giác ra xem sao 
                - tối nay chúng ta thử thay đổi độ sâu xem sao xem tọa độ x, y, z, w sẽ thay đổi và chúng được vẽ ra như thế nào 
            - 
        */

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./img/wall.png", &width, &height, &nrChannels, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width2, height2, nrChannels2;
    unsigned char *data2 = stbi_load("./img/happy.png", &width2, &height2, &nrChannels2, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data2);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
    // projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / SCR_HEIGHT, 1.0f, 4.0f);
    projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / SCR_HEIGHT, 1.0f, 2.0f);

    glm::vec3 p1 = glm::vec3(-1.0f, 1.0f, 0.0f);
    glm::vec3 p2 = glm::vec3(1.0f, 1.0f, -1.0f);
    glm::vec3 p3 = glm::vec3(1.0f, -1.0f, 0.0f);
    glm::vec3 p4 = glm::vec3(-1.0f, -1.0f, 0.0f);

    // vec4 pos = gl_Position = vec4(2.0, 0.5, 0.0, 2.0);
    // // Sau perspective divide:
    // NDC = vec3(pos.x / pos.w, pos.y / pos.w, pos.z / pos.w)
    //     = vec3(2.0 / 2.0, 0.5 / 2.0, 0.0 / 2.0)
    //     = vec3(1.0, 0.25, 0.0) -> các điểm ảnh có tọa độ nằm trong khoảng [-1, 1] thì openGL mới vẽ
    // các điểm ảnh có tọa độ nằm ngoài khoảng [-1, 1] thì openGL sẽ không vẽ điểm ảnh đó 

    outVectorFormatted(projection * view * model * glm::vec4(p1, 1.0));
    outVectorFormatted(projection * view * model * glm::vec4(p2, 1.0));
    outVectorFormatted(projection * view * model * glm::vec4(p3, 1.0));
    outVectorFormatted(projection * view * model * glm::vec4(p4, 1.0));

    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.setFloat("mixValue", mixValue);

        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE1);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f;
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f;
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
