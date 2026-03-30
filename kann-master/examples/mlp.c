
//mse 方差最小算法
//mlp 输入层无激活函数，损失函数各有优劣
//池化跟卷积的异同？
// RNN 相比 Transformer 的最大劣势,无法并行计算
//现在提到 RNN，通常指的就是 LSTM 或 GRU，原始 RNN 已很少单独使用。
//Long Short-Term Memory GRU 是 LSTM 的简化版，它将遗忘门和输入门合并为更新门
//在 RNN/LSTM/GRU 内部，Sigmoid 负责做“开关”， tanh 负责做“内容
#if 0
#include "../Getopt-for-Visual-Studio-master/getopt.h"
#include <stdlib.h>
//#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include "../kann.h"
#include "../kautodiff.h"
#include "../kann_extra/kann_data.h"
#include <Windows.h>


static kann_t *model_gen(int n_in, int n_out, int loss_type, int n_h_layers, int n_h_neurons, float h_dropout)
{
	int i;
	kad_node_t *t;
	t = kann_layer_input(n_in);
	for (i = 0; i < n_h_layers; ++i)
		t = kann_layer_dropout(kad_relu(kann_layer_dense(t, n_h_neurons)), h_dropout);
	return kann_new(kann_layer_cost(t, n_out, loss_type), 0);
}


// to compile and run: gcc -O2 this-prog.c kann.c kautodiff.c -lm && ./a.out
#include <stdlib.h>
#include <stdio.h>
#include "../kann.h"

int test(void)
{
	int i, k, max_bit = 20, n_samples = 30000, mask = (1 << max_bit) - 1, n_err, max_k;
	float** x, ** y, max, * x1;
	kad_node_t* t;
	kann_t* ann;
	// construct an MLP with one hidden layers
	t = kann_layer_input(max_bit);
	for (int i = 0; i < 1; ++i)
		t = kad_relu(kann_layer_dense(t, 64));
	t = kann_layer_cost(t, max_bit + 1, KANN_C_CEM); // output uses 1-hot encoding
	ann = kann_new(t, 0);
	// generate training data
	x = (float**)calloc(n_samples, sizeof(float*));
	y = (float**)calloc(n_samples, sizeof(float*));
	for (i = 0; i < n_samples; ++i) {
		int c, a = kad_rand(0) & (mask >> 1);
		x[i] = (float*)calloc(max_bit, sizeof(float));
		y[i] = (float*)calloc(max_bit + 1, sizeof(float));
		for (k = c = 0; k < max_bit; ++k)
			x[i][k] = (float)(a >> k & 1), c += (a >> k & 1);
		y[i][c] = 1.0f; // c is ranged from 0 to max_bit inclusive
	}
	// train
	kann_train_fnn1(ann, 0.001f, 64, 20, 10, 0.1f, n_samples, x, y);
	// predict
	x1 = (float*)calloc(max_bit, sizeof(float));
	for (i = n_err = 0; i < n_samples; ++i) {
		int c, a = kad_rand(0) & (mask >> 1); // generating a new number
		const float* y1;
		for (k = c = 0; k < max_bit; ++k)
			x1[k] = (float)(a >> k & 1), c += (a >> k & 1);
		y1 = kann_apply1(ann, x1);
		for (int i = 0; i < max_bit + 1; ++i) {
			//printf("y1[%d]:%f\r\n", i, y1[i]);
		}
		int v = (int)y1[c];
		if (v == 0) {
			//printf("Truth: %d; Predicted: %lf\r\n", c, y1[c]);
		}
		
		for (k = 0, max_k = -1, max = -1.0f; k <= max_bit; ++k) // find the max
			if (max < y1[k]) 
				max = y1[k], max_k = k;
		if (max_k != c)
			++n_err;
	}
	fprintf(stderr, "Test error rate: %.2f%%\n", 100.0 * n_err / n_samples);
	kann_delete(ann); // TODO: also to free x, y and x1
	return 0;
}

int* g_i1[16] = { 0 };

__int64 test64bit() {


}


struct MyString {
	char* str;		//分配地址
	int unused[3];
	int len;		//字符串实际长度
	int maxlen;		//分配大小
 };

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <Windows.h>

// 屏幕尺寸
#define WIDTH 80
#define HEIGHT 40

// 3D 立方体的 8 个顶点
typedef struct {
	float x, y, z;
} Point3D;

Point3D cube[8] = {
	{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
	{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
};

// 立方体的 12 条边（连接两个顶点的索引）
int edges[12][2] = {
	{0,1}, {1,2}, {2,3}, {3,0},  // 背面
	{4,5}, {5,6}, {6,7}, {7,4},  // 正面
	{0,4}, {1,5}, {2,6}, {3,7}   // 连接线
};

// 旋转角度
float angle = 0;

// 将 3D 点投影到 2D 屏幕
void project(Point3D p, int* x, int* y) {
	// 简单的透视投影
	float distance = 5;
	float factor = distance / (distance + p.z);

	// 投影到屏幕坐标
	*x = (int)(p.x * factor * 20 + WIDTH / 2);
	*y = (int)(-p.y * factor * 10 + HEIGHT / 2);
}

// 绕 Y 轴旋转
Point3D rotateY(Point3D p, float angle) {
	Point3D result;
	float cosA = cos(angle);
	float sinA = sin(angle);

	result.x = p.x * cosA - p.z * sinA;
	result.y = p.y;
	result.z = p.x * sinA + p.z * cosA;

	return result;
}

// 绕 X 轴旋转
Point3D rotateX(Point3D p, float angle) {
	Point3D result;
	float cosA = cos(angle);
	float sinA = sin(angle);

	result.x = p.x;
	result.y = p.y * cosA - p.z * sinA;
	result.z = p.y * sinA + p.z * cosA;

	return result;
}

// 绘制立方体
void draw_cube(float angleX, float angleY) {
	char screen[HEIGHT][WIDTH + 1];  // +1 for null terminator
	float zbuffer[HEIGHT][WIDTH];

	// 清空屏幕和 Z-buffer
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			screen[i][j] = ' ';
			zbuffer[i][j] = -1000;  // 初始化为很小的值
		}
		screen[i][WIDTH] = '\0';
	}

	// 旋转所有顶点
	Point3D rotated[8];
	for (int i = 0; i < 8; i++) {
		rotated[i] = rotateX(cube[i], angleX);
		rotated[i] = rotateY(rotated[i], angleY);
	}

	// 计算投影坐标
	int projX[8], projY[8];
	for (int i = 0; i < 8; i++) {
		project(rotated[i], &projX[i], &projY[i]);
	}

	// 绘制每条边
	for (int i = 0; i < 12; i++) {
		int p1 = edges[i][0];
		int p2 = edges[i][1];

		int x1 = projX[p1];
		int y1 = projY[p1];
		int x2 = projX[p2];
		int y2 = projY[p2];

		// 使用 Bresenham 算法画线
		int dx = abs(x2 - x1);
		int dy = -abs(y2 - y1);
		int sx = x1 < x2 ? 1 : -1;
		int sy = y1 < y2 ? 1 : -1;
		int err = dx + dy;

		int x = x1;
		int y = y1;

		while (1) {
			// 检查边界
			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
				// 使用简单的 Z 值决定亮暗
				float z = (rotated[p1].z + rotated[p2].z) / 2;
				if (z > zbuffer[y][x]) {
					zbuffer[y][x] = z;
					// 根据深度选择不同的字符
					if (z > 2) screen[y][x] = '@';
					else if (z > 1) screen[y][x] = '#';
					else if (z > 0) screen[y][x] = '*';
					else if (z > -1) screen[y][x] = '.';
					else screen[y][x] = ' ';
				}
			}

			if (x == x2 && y == y2) break;

			int e2 = 2 * err;
			if (e2 >= dy) {
				err += dy;
				x += sx;
			}
			if (e2 <= dx) {
				err += dx;
				y += sy;
			}
		}
	}

	// 绘制屏幕
	printf("\033[H");  // 移动光标到左上角
	for (int i = 0; i < HEIGHT; i++) {
		printf("%s\n", screen[i]);
	}
}

int mymytest() {
	float angleX = 0, angleY = 0;

	// 隐藏光标
	printf("\033[?25l");

	while (1) {
		draw_cube(angleX, angleY);

		// 更新旋转角度
		angleX += 0.03;
		angleY += 0.05;

		// 控制帧率 (Linux/Unix)
		Sleep(50);  // 50ms = 20 FPS

		// 如果在 Windows 下，用 Sleep(50)
	}

	// 恢复光标
	printf("\033[?25h");

	return 0;
}

void mytest() {
	
	FILE * fp = _wfopen(L"c:\\Users\\liujinguang01\\Desktop\\doc\\..\\test", L"r");
	mymytest();


	int a = 0;
	int b = 0;
	a,b = test64bit();
	printf("%x %x\r\n", a, b);

	HANDLE f = CreateFileA("c:\\Users\\liujinguang01\\Desktop\\doc\\..\\test", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//HANDLE f = CreateFileA("c:\\Users\\liujinguang01\\Desktop\\test",GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(f == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "CreateFileA failed: %d\n", GetLastError());
		return;
	}

	CloseHandle(f);
}

int main (int argc, char* argv[])
{
	//mytest();

	test();

	int max_epoch = 50, mini_size = 64, max_drop_streak = 10, loss_type = KANN_C_CEB;
	int i, j, c, n_h_neurons = 64, n_h_layers = 1, seed = 11, n_threads = 1;
	kann_data_t *in = 0;
	kann_t *ann = 0;
	char *out_fn = 0, *in_fn = 0;
	float lr = 0.001f, frac_val = 0.1f, h_dropout = 0.0f;

	while ((c = getopt(argc, argv, "n:l:s:r:m:B:o:i:d:v:Mt:")) >= 0) {
		if (c == 'n') n_h_neurons = atoi(optarg);
		else if (c == 'l') n_h_layers = atoi(optarg);
		else if (c == 's') seed = atoi(optarg);
		else if (c == 'i') in_fn = optarg;
		else if (c == 'o') out_fn = optarg;
		else if (c == 'r') lr = atof(optarg);
		else if (c == 'm') max_epoch = atoi(optarg);
		else if (c == 'B') mini_size = atoi(optarg);
		else if (c == 'd') h_dropout = atof(optarg);
		else if (c == 'v') frac_val = atof(optarg);
		else if (c == 'M') loss_type = KANN_C_CEM;
		else if (c == 't') n_threads = atoi(optarg);
	}
	if (argc - optind < 1) {
		FILE *fp = stdout;
		fprintf(fp, "Usage: mlp [options] <in.knd> [truth.knd]\n");
		fprintf(fp, "Options:\n");
		fprintf(fp, "  Model construction:\n");
		fprintf(fp, "    -i FILE     read trained model from FILE []\n");
		fprintf(fp, "    -o FILE     save trained model to FILE []\n");
		fprintf(fp, "    -s INT      random seed [%d]\n", seed);
		fprintf(fp, "    -l INT      number of hidden layers [%d]\n", n_h_layers);
		fprintf(fp, "    -n INT      number of hidden neurons per layer [%d]\n", n_h_neurons);
		fprintf(fp, "    -d FLOAT    dropout at the hidden layer(s) [%g]\n", h_dropout);
		fprintf(fp, "    -M          use multi-class cross-entropy (binary by default)\n");
		fprintf(fp, "  Model training:\n");
		fprintf(fp, "    -r FLOAT    learning rate [%g]\n", lr);
		fprintf(fp, "    -m INT      max number of epochs [%d]\n", max_epoch);
		fprintf(fp, "    -B INT      mini-batch size [%d]\n", mini_size);
		fprintf(fp, "    -v FLOAT    fraction of data used for validation [%g]\n", frac_val);
		fprintf(fp, "    -t INT      number of threads [%d]\n", n_threads);
		return 1;
	}
	if (argc - optind == 1 && in_fn == 0) {
		fprintf(stderr, "ERROR: please specify a trained model with option '-i'.\n");
		return 1;
	}
	//-r 0.1 -m 3 -B 100 -v 80 -t 1 -o mnist-mlp.kan kann-data/mnist-train-x.knd.gz

	kad_trap_fe();
	kann_srand(seed);
	in = kann_data_read(argv[optind]);
	if (in_fn) {
		ann = kann_load(in_fn);
		assert(kann_dim_in(ann) == in->n_col);
	}

	if (optind+1 < argc) { // train
		kann_data_t *out;
		out = kann_data_read(argv[optind+1]);
		assert(in->n_row == out->n_row);
		if (ann) assert(kann_dim_out(ann) == out->n_col);
		else ann = model_gen(in->n_col, out->n_col, loss_type, n_h_layers, n_h_neurons, h_dropout);
		if (n_threads > 1) kann_mt(ann, n_threads, mini_size);
		kann_train_fnn1(ann, lr, mini_size, max_epoch, max_drop_streak, frac_val, in->n_row, in->x, out->x);
		if (out_fn) kann_save(out_fn, ann);
		kann_data_free(out);
	} else { // apply
		int n_out;
		if (in->cname) {
			printf("#sample");
			for (i = 0; i < in->n_col; ++i)
				printf("\t%s", in->cname[i]);
			printf("\n");
		}
		kann_switch(ann, 0);
		n_out = kann_dim_out(ann);
		for (i = 0; i < in->n_row; ++i) {
			const float *y;
			y = kann_apply1(ann, in->x[i]);
			if (in->rname) printf("%s\t", in->rname[i]);
			for (j = 0; j < n_out; ++j) {
				if (j) putchar('\t');
				printf("%.3g", y[j] + 1.0f - 1.0f);
			}
			putchar('\n');
		}
	}

	kann_delete(ann);
	kann_data_free(in);
	return 0;
}
#endif