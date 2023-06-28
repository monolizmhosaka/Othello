#include <iostream>



//盤面 空白(0) 黒(-1) 白(1) 番兵(2)
int board[10][10] = {};
//手番
int player = -1;

//盤面の生成
void make_board() {
	//番兵
	for (int i = 0; i < 10; i++) {
		board[0][i] = 2;
		board[9][i] = 2;
		board[i][0] = 2;
		board[i][9] = 2;
	}
	//基本位置
	board[4][4] = 1;
	board[5][5] = 1;
	board[4][5] = -1;
	board[5][4] = -1;
}

//盤面の表示
void show_board() {
	//番兵も含めて表示
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			switch (board[i][j]) {
			case -1:
				std::cout << "●";
				break;
			case 1:
				std::cout << "○";
				break;
			case 0:
				std::cout << "-";
				break;
			case 2:
				//std::cout << "~";
				break;
			default:
				break;
			}
		}
		std::cout << std::endl;
	}
}

//手番の表示
void show_player() {
	switch (player) {
	case -1:
		std::cout << "先手(黒)の手番です" << std::endl;
		break;
	case 1:
		std::cout << "後手(白)の手番です" << std::endl;
		break;
	default:
		//std::cout << "error" << std::endl;
		break;
	}
}

//特定の座標から特定の方向に挟めるか判定
int check_dir(int i, int j, int dir_i, int dir_j) {
	//指定方向に相手の石がある場合は次のマスを探索する
	int times = 1;
	while (board[i + dir_i * times][j + dir_j * times] == player * -1) {
		times++;
	}
	//指定方向の最後に自分の石がある場合
	if (board[i + dir_i * times][j + dir_j * times] == player) {
		//指定方向に相手の石が何個あるかを返す
		return times - 1;
	}
	//指定方向の最後に自分の石がなければ0を返す
	return 0;
}

//特定の場所に置くことができるか判定
bool check_plc(int i, int j) {
	//場所が空であるかどうか
	if (board[i][j] == 0) {
		//全方向を探索
		for (int dir_i = -1; dir_i < 2; dir_i++) {
			for (int dir_j = -1; dir_j < 2; dir_j++) {
				if (check_dir(i, j, dir_i, dir_j)) {
					//配置可能であればtrueを返す
					return true;
				}
			}
		}
	}
	return false;
}

//終了判定
bool flag_fin() {
	//置ける場所があるか判定
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			if (check_plc(i, j)) {
				return true;
			}
		}
	}

	//プレイヤーを変えて置ける場所があるか判定
	player *= -1;
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			if (check_plc(i, j)) {
				std::cout << "置く場所がないためPlayerを変更しました" << std::endl;
				return true;
			}
		}
	}

	return false;
}

//石を配置する
void place_stn(int i, int j) {
	//方向毎に走査
	for (int dir_i = -1; dir_i < 2; dir_i++) {
		for (int dir_j = -1; dir_j < 2; dir_j++) {
			//挟んだ石の数
			int change_num = check_dir(i, j, dir_i, dir_j);
			//挟んだ石の数だけ置き換える
			for (int k = 1; k < change_num + 1; k++) {
				board[i + dir_i * k][j + dir_j * k] = player;
			}
		}
	}
	//配置箇所を置き換える
	board[i][j] = player;
}

//勝敗判定
void judge_board() {
	int count_b = 0; //黒石の数
	int count_w = 0; //白石の数
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			if (board[i][j] == -1) {
				count_b++;
			}
			else if (board[i][j] == 1) {
				count_w++;
			}
		}
	}
	//結果表示
	std::cout << "先手" << count_b << ":後手" << count_w << std::endl;
	//勝敗判定
	if (count_b > count_w) {
		std::cout << "先手の勝利" << std::endl;
	}
	else if (count_w > count_b) {
		std::cout << "後手の勝利" << std::endl;
	}
	else {
		std::cout << "引き分け" << std::endl;
	}
}

int main() {
	//盤面の生成
	make_board();
	//終了までループ
	while (flag_fin()) {
		system("cls");

		//盤面の表示
		show_board();
		//手番の表示
		show_player();
		//入力受付
		int i, j;
		do {
			std::cout << "配置場所を入力してください" << std::endl;
			std::cin >> i >> j;
		} while (!check_plc(i, j));
		//石を配置する
		place_stn(i, j);
		//手番を入れ替える
		player *= -1;
	}
	//盤面の表示
	show_board();
	//勝利判定
	judge_board();
	return 0;
}