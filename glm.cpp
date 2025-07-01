#include <iostream>
#include <iomanip>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

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
	cout << fixed << setprecision(2);
	for (int i = 0; i < length; i++) cout << "[" << (float)V[i] << "]\n";
	cout << "[.]\n";
	return;
}

namespace VectorTransform {
	inline static void scale(glm::vec4& in, glm::vec3 ratio) {
		glm::mat4 v_Translate = glm::scale(glm::mat4(1.0f), ratio);
		in = v_Translate * in;
	}
	inline static void translate(glm::vec4& in, glm::vec3 trans) {
		glm::mat4 v_Translate = glm::translate(glm::mat4(1.0f), trans);
		in = v_Translate * in;
	}
	inline static void rotate(glm::vec4& in, float angle, glm::vec3 axis) {
		glm::mat4 v_Translate = glm::rotate(glm::mat4(1.0f), angle, axis);
		in = v_Translate * in;
	}
}

int main()
{

	// Bản chất khi mình muốn biểm đổi 1 vector thì chúng ta sẽ nhân nó với 1 Ma trận 
	// Có nhiều loại ma trận để biến đổi dưới đây là 1 số loại phổ biến nhất trong openGL 

	{
		// ma trận dịch vector theo 1 thông số đó là vector dịch 
		// glm::mat4 v_Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5));
		// outMatrixFormatted(v_Translate);
		// outVectorFormatted(v_Translate * v_A);

		glm::vec4 v_A = glm::vec4(1.0f, 2.0f, 0.0f, 1.0f);
		VectorTransform::translate(v_A, glm::vec3(0.5, 0.5, 0.5));
		outVectorFormatted(v_A);
	}

	{
		// ma trận xoay vector theo 2 thông số đó là góc là trục, cái này mình chiếu hình đó lên trục sau đó tưởng tượng cái trục đó xoay 
		// cứ tưởng tượng cái trục nó xoay theo ngược chiều kim đồng hồ đi 1 góc nối 1 sợi dây vuông góc lên cái trục đó là tưởng tượng dễ nhất 
		// glm::mat4 v_Translate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); // xoay theo trục z
		// outMatrixFormatted(v_Translate);
		// outVectorFormatted(v_Translate * v_A);

		glm::vec4 v_A = glm::vec4(1.0f, 2.0f, 0.0f, 1.0f);
		VectorTransform::rotate(v_A, glm::radians(90.0f), glm::vec3(0.0, 0.0, 0.1));
		outVectorFormatted(v_A);
	}

	{
		// ma trận phóng to, thu nhỏ vector cái này khá dễ hiểu chỉ đơn giản là tọa đọ của nó nhân với tỉ lệ đó mà thôi 
		// glm::mat4 v_Translate = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5)); // scale x, y, z = 1/2
		// outMatrixFormatted(v_Translate);
		// outVectorFormatted(v_Translate * v_A);

		glm::vec4 v_A = glm::vec4(1.0f, 2.0f, 0.0f, 1.0f);
		VectorTransform::scale(v_A, glm::vec3(0.5, 0.5, 0.9));
		outVectorFormatted(v_A);
	}

	// Chứng minh khá lằng nhằng và có vẻ không cần thiết với quy mô như này 
	// Hiểu các hàm hoạt động như nào là được bạn có thể đọc API để hiểu hơn cách hoạt động của chúng 

	return 0;
}

