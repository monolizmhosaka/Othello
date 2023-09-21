﻿#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<time.h>
#include<stdlib.h>



//////////////////////////////////////////////////
//グローバル変数の定義
//////////////////////////////////////////////////

//boardは盤面の状況,先手(-1),後手(1),空(0),番兵(2)
int board[10][10];

//playerは先手(-1)後手(1)
int player = -1;



//////////////////////////////////////////////////
//盤面生成
//////////////////////////////////////////////////

void make_board() {

	//変数boardに8*8行列,要素全て0を入力
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			board[i][j] = 0;
		}
	}

	//盤外に2を入力
	for (int k = 0; k < 10; k++) {
		board[0][k] = 2;
		board[k][0] = 2;
		board[9][k] = 2;
		board[k][9] = 2;
	}

	//変数boardに石の初期値を入力
	board[4][4] = 1;
	board[5][5] = 1;
	board[4][5] = -1;
	board[5][4] = -1;
}



//////////////////////////////////////////////////
//特定の座標,方向に相手の石を挟めるか判定
//////////////////////////////////////////////////

int pos_dir(int row, int column, int dir_r, int dir_c, int color) {

	//指定方向に相手の石がない場合は0を返す
	if (!(board[row + dir_r][column + dir_c] == (color * -1))) {
		return 0;
	}

	//指定方向に相手の石がある場合は次のマスを探索する
	int times = 1;
	while (board[row + (dir_r * times)][column + (dir_c * times)] == (color * -1)) {
		times++;
	}

	//指定方向の最後に自分の石がなければ0を返す
	if (!(board[row + (dir_r * times)][column + (dir_c * times)] == color)) {
		return 0;
	}

	//指定方向に相手の石が何個あるかを返す
	return times - 1;
}



//////////////////////////////////////////////////
//特定の場所に手番の石を配置可能か判定
//////////////////////////////////////////////////

int pos_place(int row, int column, int color) {

	//場所が空であるかどうか
	if (board[row][column] == 0) {

		//全方向を探索
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (pos_dir(row, column, i, j, color)) {

					//配置可能であれば1を返す
					return 1;
				}
			}
		}
	}
	return 0;
}



//////////////////////////////////////////////////
//終了判定
//////////////////////////////////////////////////

int flag() {

	//全ての場所における場所があるかどうか
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			if (pos_place(i, j, player)) {
				return 0;
			}
		}
	}

	//プレイヤーを変えて全ての場所における場所があるかどうか
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			if (pos_place(i, j, (player * -1))) {
				player *= -1;
				printf("置ける場所がないためplayerを変更しました。\n");
				return 0;
			}
		}
	}

	return 1;
}



//////////////////////////////////////////////////
//盤面表示
//////////////////////////////////////////////////

void show_board() {

	//ループで各行を1列ずつ表示
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			switch (board[i][j]) {
			case -1:
				printf("●");
				break;
			case 1:
				printf("○");
				break;
			case 0:
				printf("-");
				break;
			case 2:
				//printf("~");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
}



//////////////////////////////////////////////////
//手入力
//////////////////////////////////////////////////

void get_data(int* row, int* column) {

	printf("入力してください\n");
	scanf("%d%d", row, column);
}



//////////////////////////////////////////////////
//簡易AI(乱数)
//////////////////////////////////////////////////

void ai_random(int* row, int* column) {

	//乱数から1から8までの組み合わせを作り出す
	*row = rand() % 8 + 1;
	*column = rand() % 8 + 1;
}



//////////////////////////////////////////////////
//石を配置
//////////////////////////////////////////////////

void put_board(int row, int column) {

	//石を変える方向を探索
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {

			//石を変える必要がある時,変える個数がnumに入る
			int num = pos_dir(row, column, i, j, player);
			if (num) {

				//変えられる時,numに入った個数だけ順番に手番の石にする
				for (int k = 1; k < num + 1; k++) {
					board[row + (k * i)][column + (k * j)] = player;
				}
			}
		}
	}

	//石を追加する
	board[row][column] = player;
}



//////////////////////////////////////////////////
//一手分実行
//////////////////////////////////////////////////

int do_round() {

	int row = 0;
	int column = 0;

	//アドレスで座標を取って来る
	switch (player) {
	case -1:
		printf("先手の手番です。\n");
		get_data(&row, &column);
		break;
	case 1:
		printf("後手の手番です。\n");
		get_data(&row, &column);
		break;
	default:
		break;
	}

	//配置可能であれば配置
	if (pos_place(row, column, player)) {
		printf("%d%dに配置します。\n", row, column);
		put_board(row, column);
		return 0;
	}

	//配置不可能であればもう一度入力させる
	printf("%d%dには配置できません。\n", row, column);
	
	// 再帰で再入力を促すのはメモリリークするのでよくないぜ
	return do_round();
}



//////////////////////////////////////////////////
//ゲームの繰り返し実行
//////////////////////////////////////////////////
void loop_game() {

	while (1) {

		//終了判定
		if (flag()) {
			break;
		}

		//盤面の表示
		show_board();

		//配置
		do_round();

		//手番交代
		player *= -1;
	}
}



//////////////////////////////////////////////////
//結果表示
//////////////////////////////////////////////////

void score() {

	//countは石の数を数える
	int count_b = 0;
	int count_w = 0;

	//全探索
	for (int i = 1; i < 9; i++) {
		for (int j = 1; j < 9; j++) {
			if (board[i][j] == -1) {
				count_b++;
			}
			if (board[i][j] == 1) {
				count_w++;
			}
		}
	}

	//盤面の表示
	show_board();

	//結果の表示
	printf("先手%d石,後手%d石\n", count_b, count_w);
	if (count_b > count_w) {
		printf("先手の勝利\n");
	}
	else if (count_b < count_w) {
		printf("後手の勝利\n");
	}
	else {
		printf("引き分け\n");
	}
}



//////////////////////////////////////////////////
//メイン関数
//////////////////////////////////////////////////

int main(void) {

	//乱数用
	srand((int)time(NULL));

	printf("GameStart\n");

	//盤面を作る
	make_board();

	//繰り返し部分
	loop_game();

	printf("GameEnd\n");

	score();

	return 0;
}
