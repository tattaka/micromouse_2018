/*
 * maze_solve.c
 *
 *  Created on: 2017/08/13
 *      Author: tattaka
 */

#include "moter.h"
#include "maze_solve.h"
#include <stdlib.h>
#include <stdarg.h>

unsigned int white_data[MAZESIZE_X*MAZESIZE_Y] = {0}; //プログラム内の迷路データ
unsigned char dire_solve[MAZESIZE_X*MAZESIZE_Y] = {0};

int step = 0;

int search(int x, int y, int goal_num, ...){ //(int 開始地点x, int 開始地点y, int ゴールの数, ...)
	int i = 0;
	int goal[5];
	int agent = 0; //現在の場所を保存する変数
	unsigned char dire; //実際に進む方向を保存する変数
	va_list goals;
	if(goal_num<1) {
		return 0;
	}
	va_start(goals, goal_num);
	for(i = 0; i < goal_num; i++) {
		goal[i] = va_arg(goals, int);
	}
	va_end(goals);
	agent = x + y * MAZESIZE_X; //現在地を開始地点に合わせる

	map_init(x, y, 0, 5, goal[0], goal[1], goal[2], goal[3], goal[4]); //迷路情報を最適化する

	while((white_data[agent]>>8) != 0) { //現在地の歩数マップが0でない時（ゴールじゃない時）
		/*
		 * 現在地の壁情報を追加する
		 * */
		if((agent/MAZESIZE_X) > 0) { //迷路の下端行ではない場合
			/*
			 * white_data[agent-MAZESIZE_X] =
			 * 迷路の下端行ではない場合、下壁があれば下のマスのデータに0x2を追加する
			 */
		}
		if((agent%MAZESIZE_X) > 0) { //迷路の左端行ではない場合
			/*
			 * white_data[agent-1] =
			 * 迷路の左端行ではない場合、左壁があれば左のマスのデータに0x1を追加する
			 */
		}
		if((white_data[agent]&0x04) != 0x04) { //未知区間の場合今いる座標を既知区間にする
			white_data[agent] = (white_data[agent]|0x04);
			//data[agent] = (data[agent]|0x0004);
		}
		/*
                   歩数マップに従う通り道が開いている場合進む（開いている道の一つが歩数マップの予想方向と一致していれば）
                   開いていない場合（予想方向のマスが開いてない場合）
                   歩数マップを更新する
                   予想方向に進む
		 */
		dire = 0;
		/*進めるか進めないかを4方向で判断*/
		if((white_data[agent]&0x02)==0) { //上壁が開いている場合
			dire = (dire|0x08);
		}
		if((white_data[agent]&0x01)==0) { //右壁が開いている場合
			dire = (dire|0x01);
		}
		if((agent/MAZESIZE_X) > 0) {
			if((white_data[agent-MAZESIZE_X]&0x02)==0) { //下壁が開いている場合
				dire = (dire|0x04);
			}
		}
		if((agent%MAZESIZE_X) > 0) {
			if((white_data[agent-1]&0x01)==0) { //左壁が開いている場合
				dire = (dire|0x02);
			}
		}
		if((dire&((white_data[agent]&0xF0)>>4))==0) { //予想方向が進めない場合
			map_init(agent%MAZESIZE_X, agent/MAZESIZE_X, 0, 5, goal[0], goal[1], goal[2], goal[3], goal[4]);
			dire = 0; //方向変数をクリーン
			dire = ((white_data[agent]&0xF0)>>4); //新しく計算した方向を方向変数に保存
			if((dire&0x08)==0x08) { //方向変数が上側の場合
				agent = agent + MAZESIZE_X; //上に移動
				/*
				 * 進行方向の場合180mm進む
				 * 進行方向の逆の場合、前壁調整後180度回転して180mm進んでマウスの方向変数を書き換える
				 * 進行方向の左の場合45度スラロームして80mm進んでマウスの方向変数を書き換える
				 * 進行方向の右の場合-45度スラロームして80mm進んでマウスの方向変数を書き換える
				 */
			}
			else if((dire&0x04)==0x04) { //方向変数が下側の場合
				agent = agent - MAZESIZE_X; //下側に移動
			}
			else if((dire&0x02)==0x02) { //方向変数が左側の場合
				agent = agent - 1; //左側に移動
			}
			else if((dire&0x01)==0x01) { //方向変数が右側の場合
				agent = agent + 1; //右側に移動
			}
		}
		else{
			dire = ((white_data[agent]&0xF0)>>4); //予想方向に方向を確定
			if((dire&0x08)==0x08) {
				agent = agent + MAZESIZE_X;
			}
			else if((dire&0x04)==0x04) {
				agent = agent - MAZESIZE_X;
			}
			else if((dire&0x02)==0x02) {
				agent = agent - 1;
			}
			else if((dire&0x01)==0x01) {
				agent = agent + 1;
			}
		}
		step = step + 1;
	}
	return step;
};

int solve(int x, int y, int goal_num, ...){ //(int 開始地点x, int 開始地点y, int ゴールの数, ...)
	int i = 0;
	int goal[5];
	int agent = 0; //現在の場所を保存する変数

	va_list goals;
	if(goal_num<1) {
		return 0;
	}
	va_start(goals, goal_num);
	for(i = 0; i < goal_num; i++) {
		goal[i] = va_arg(goals, int);
	}
	va_end(goals);
	agent = x + y * MAZESIZE_X; //現在地を開始地点に合わせる

	map_init(x, y, 1, 5, goal[0], goal[1], goal[2], goal[3], goal[4]); //迷路情報を最適化する

	while((white_data[agent]>>8) != 0) { //現在地の歩数マップが0でない時（ゴールじゃない時）
		/*
                   歩数マップに従う通り道が開いている場合進む
		 */
		dire_solve[agent] = ((dire_solve[agent]&0xF0)>>4); //予想方向に方向を確定
		if((dire_solve[agent]&0x08)==0x08) {
			agent = agent + MAZESIZE_X;
		}
		else if((dire_solve[agent]&0x04)==0x04) {
			agent = agent - MAZESIZE_X;
		}
		else if((dire_solve[agent]&0x02)==0x02) {
			agent = agent - 1;
		}
		else if((dire_solve[agent]&0x01)==0x01) {
			agent = agent + 1;
		}
		step = step + 1;
	}
	return step;
};

void map_init(int x, int y, int mode, int goal_num, ...){ //現在の座標、ゴール座標の配列、モード
	int i = 0; //ループ変数
	int j = 0;
	int k = 0;

	int head = 0; //更新判断変数
	int tail = 1;

	int temp = 255; //ソート用変数
	int buffx = 0; //1つ前の予想エージェントx座標
	int buffy = 0; //1つ前の予想エージェントy座標
	int goal[5];
	va_list goals;
	if(goal_num<1) return;

	va_start(goals, goal_num);
	for(i = 0; i < goal_num; i++) {
		goal[i] = va_arg(goals, int);
	}
	va_end(goals);

	i = 0;
	if(mode == 0) {//探索モードの場合
		while(1) {
			white_data[i] = (white_data[i]|0xFF00); //歩数マップを255で初期化
			white_data[i] = (white_data[i]&0xFF0F); //方向変数を0で初期化
			i++;
			if(i > (MAZESIZE_X*MAZESIZE_Y-1)) { //初期化終了
				break;
			}
		}

		i = 0;
		while(1) { //ゴール座標を0で初期化
			white_data[goal[i]] = (white_data[goal[i]]&0x000F);
			i++;
			if(goal[i] == 0xFFFF) {
				break;
			}
		}
		//更新されたされないの条件は現在のターゲット歩数より大きいマスが見つかったら未更新マスが残っていると考える

		j = 0; //現在更新中の歩数の値
		//iは現在更新中の座標
		while(head != tail) {
			for(i = 0; i < MAZESIZE_X*MAZESIZE_Y; i++) {
				if((white_data[i]>>8)==j) { //現在のマスの歩数が現在更新中の歩数である場合
					if((i/MAZESIZE_X)<MAZESIZE_Y-1) { //上側にマスがある時
						if((white_data[i]&0x2)!=0x2) { //上のマスと繋がっているなら
							if((white_data[i+MAZESIZE_X]>>8)>j+1) { //現在のマスの上マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i+MAZESIZE_X] = (white_data[i+MAZESIZE_X]&(((j+1)<<8)|0x0F)); //新しく歩数を代入
							}
						}
					}
					if((i%MAZESIZE_X)<MAZESIZE_X-1) { //右側にマスがある時
						if((white_data[i]&0x1)!=0x1) { //右のマスと繋がっていたら
							if((white_data[i+1]>>8)>j+1) {//現在のマスの右マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i+1] = (white_data[i+1]&(((j+1)<<8)|0x0F));
							}
						}
					}
					if((i/MAZESIZE_X)>0) { //下にマスがある時
						if((white_data[i-MAZESIZE_X]&0x2)!=0x2) { //下のマスと繋がっていたら
							if((white_data[i-MAZESIZE_X]>>8)>j+1) {//現在のマスの下マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i-MAZESIZE_X] = (white_data[i-MAZESIZE_X]&(((j+1)<<8)|0x0F));
							}
						}
					}
					if((i%MAZESIZE_X)>0) { //左にマスがある場合
						if((white_data[i-1]&0x1)!=0x1) { //左のマスと繋がっていたら
							if((white_data[i-1]>>8)>j+1) {//現在のマスの左マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i-1] = (white_data[i-1]&(((j+1)<<8)|0x0F));
							}
						}
					}
				}
			}
			tail = 0;
			for(k = 0; k < MAZESIZE_X*MAZESIZE_Y; k++) { //現在のターゲット歩数より大きいマスが見つかった場合未更新
				if(j<(white_data[k]>>8)) {
					tail = 1;
				}
			}
			j++;
		}

		i = x + y * MAZESIZE_X;

		while((white_data[i]>>8) != 0) {
			//現在の位置からゴールまでのマスまで予想方向を書き込む
			temp = (white_data[i]>>8);
			if((i/MAZESIZE_X) < MAZESIZE_Y-1) { //上のマスの歩数が現在のマスの歩数より小さい場合上の方向のフラグを立てる
				if((white_data[i]&0x02) != 0x02) {
					if((white_data[i+MAZESIZE_X]>>8) < temp) {
						white_data[i] = (white_data[i]|0x0080)&0xFF8F;
						temp = (white_data[i+MAZESIZE_X]>>8);
					}
					else if((white_data[i+MAZESIZE_X]>>8) == temp) {
						white_data[i] = (white_data[i]|0x0080);
					}
				}
			}
			if((i/MAZESIZE_X) > 0) {
				if((white_data[i-MAZESIZE_X]&0x02) != 0x02) {
					if((white_data[i-MAZESIZE_X]>>8) < temp) { //下のマスの歩数がさっきの保存した歩数より小さい場合、下のフラグを立ててそれ以外のフラグを消す
						white_data[i] = (white_data[i]|0x0040)&0xFF4F;
						temp = (white_data[i-MAZESIZE_X]>>8);
					}
					else if((white_data[i-MAZESIZE_X]>>8) == temp) {
						white_data[i] = (white_data[i]|0x0040);
					}
				}
			}
			if((i%MAZESIZE_X) > 0) {
				if((white_data[i-1]&0x01) != 0x01) {
					if((white_data[i-1]>>8) < temp) { //左のマスの歩数がさっきの保存した歩数より小さい場合、左のフラグを立ててそれ以外のフラグを消す
						white_data[i] = (white_data[i]|0x0020)&0xFF2F;
						temp = (white_data[i-1]>>8);
					}
					else if((white_data[i-1]>>8) == temp) {
						white_data[i] = (white_data[i]|0x0020);
					}
				}
			}
			if((i%MAZESIZE_X) < MAZESIZE_X-1) {
				if((white_data[i]&0x01) != 0x01) {
					if((white_data[i+1]>>8) < temp) { //右のマスの歩数がさっきの保存した歩数より小さい場合、右のフラグを立ててそれ以外のフラグを消す
						white_data[i] = (white_data[i]|0x0010)&0xFF1F;
						temp = (white_data[i+1]>>8);
					}
					else if((white_data[i+1]>>8) == temp) {
						white_data[i] = (white_data[i]|0x0010);
					}
				}
			}
			//未知区間優先の処理を入れる
			if((white_data[i]&0x0080) == 0x0080) { //上のフラグが立っている場合
				if((white_data[i+MAZESIZE_X]&0x04) == 0) { //上のマスが未知の場合
					white_data[i] = (white_data[i]&0xFF8F); //上以外のフラグを消す
				}
			}
			if((white_data[i]&0x0040) == 0x0040) {
				if((white_data[i-MAZESIZE_X]&0x04) == 0) {
					white_data[i] = (white_data[i]&0xFF4F);
				}
			}
			if((white_data[i]&0x0020) == 0x0020) {
				if((white_data[i-1]&0x04) == 0) {
					white_data[i] = (white_data[i]&0xFF2F);
				}
			}
			if((white_data[i]&0x0010) == 0x0010) {
				if((white_data[i+1]&0x04) == 0) {
					white_data[i] = (white_data[i]&0xFF1F);
				}
			}

			if((i/MAZESIZE_X)-buffy == 1) { //進行方向と一致していたら進行方向を採用する
				if((white_data[i]&0x0080) == 0x0080) { //上のフラグが立っている場合
					white_data[i] = (white_data[i]&0xFF8F); //上以外のフラグを消す
				}
			}
			else if((i/MAZESIZE_X)-buffy == -1) {
				if((white_data[i]&0x0040) == 0x0040) {
					white_data[i] = (white_data[i]&0xFF4F);
				}
			}
			else if((i%MAZESIZE_X)-buffx == -1) {
				if((white_data[i]&0x0020) == 0x0020) {
					white_data[i] = (white_data[i]&0xFF2F);
				}
			}
			else if((i%MAZESIZE_X)-buffx == 1) {
				if((white_data[i]&0x0010) == 0x0010) {
					white_data[i] = (white_data[i]&0xFF1F);
				}
			}
			buffx =i % MAZESIZE_X;
			buffy =i / MAZESIZE_X;
			if((white_data[i]&0x0080) == 0x0080) { //これまでで残っている方向のフラグを立てる
				white_data[i] = (white_data[i]&0xFF8F);
			}
			else if((white_data[i]&0x0040) == 0x0040) {
				white_data[i] = (white_data[i]&0xFF4F);
			}
			else if((white_data[i]&0x0020) == 0x0020) {
				white_data[i] = (white_data[i]&0xFF2F);
			}
			else if((white_data[i]&0x0010) == 0x0010) {
				white_data[i] = (white_data[i]&0xFF1F);
			}

			if((white_data[i]&0x0080) == 0x0080) {
				i = i + MAZESIZE_X;
			}
			else if((white_data[i]&0x0040) == 0x0040) {
				i = i - MAZESIZE_X;
			}
			else if((white_data[i]&0x0020) == 0x0020) {
				i = i - 1;
			}
			else if((white_data[i]&0x0010) == 0x0010) {
				i = i + 1;
			}
		}
	}
	else if(mode == 1) {//単純最短の場合
		while(1) {
			white_data[i] = (white_data[i]|0xFF00); //歩数マップを255で初期化
			white_data[i] = (white_data[i]&0xFF0F); //方向変数を0で初期化
			i++;

			if((white_data[i]&0x4)==0) {
				white_data[i] = (white_data[i]|0x3);
			}

			if(i > (MAZESIZE_X*MAZESIZE_Y-1)) { //初期化終了
				break;
			}
		}

		i = 0;
		while(1) { //ゴール座標を0で初期化
			white_data[goal[i]] = (white_data[goal[i]]&0x00FF);
			i++;
			if(goal[i] == 0xFFFF) {
				break;
			}
		}
		j = 0; //現在更新中の歩数の値
		head = 0; //更新判断変数
		tail = 1;
		//iは現在更新中の座標
		while(head != tail) {
			for(i = 0; i < MAZESIZE_X*MAZESIZE_Y; i++) {
				if((white_data[i]>>8)==j) { //現在のマスの歩数が現在更新中の歩数である場合
					if((i/MAZESIZE_X)<MAZESIZE_Y-1) { //上側にマスがある時
						if((white_data[i]&0x2)!=0x2) { //上のマスと繋がっているなら
							if((white_data[i+MAZESIZE_X]>>8)>j+1) { //現在のマスの上マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i+MAZESIZE_X] = (white_data[i+MAZESIZE_X]&(((j+1)<<8)|0x0F)); //新しく歩数を代入
								if((i%MAZESIZE_X)<MAZESIZE_X-1) {
									if((white_data[i+MAZESIZE_X]&0x1)!=0x1) {
										if((white_data[i+1+MAZESIZE_X]>>8)>j+2) { //現在のマスの2つ上マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i+1+MAZESIZE_X] = (white_data[i+1+MAZESIZE_X]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
								if((i%MAZESIZE_X)>0) {
									if((white_data[i+MAZESIZE_X-1]&0x1)!=0x1) {
										if((white_data[i-1+MAZESIZE_X]>>8)>j+2) { //現在のマスの2つ上マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i-1+MAZESIZE_X] = (white_data[i-1+MAZESIZE_X]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
							}
						}
					}
					if((i%MAZESIZE_X)<MAZESIZE_Y-1) { //右側にマスがある時
						if((white_data[i]&0x1)!=0x1) { //右のマスと繋がっていたら
							if((white_data[i+1]>>8)>j+1) {//現在のマスの右マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i+1] = (white_data[i+1]&(((j+1)<<8)|0x0F));
								if((i/MAZESIZE_X)<MAZESIZE_Y-1) {
									if((white_data[i+1]&0x2)!=0x2) {
										if((white_data[i+MAZESIZE_X+1]>>8)>j+2) { //現在のマスの右上マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i+MAZESIZE_X+1] = (white_data[i+MAZESIZE_X+1]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
								if((i/MAZESIZE_X)>0) {
									if((white_data[i+1-MAZESIZE_X]&0x2)!=0x2) {
										if((white_data[i-MAZESIZE_X+1]>>8)>j+2) { //現在のマスの右上マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i-MAZESIZE_X+1] = (white_data[i-MAZESIZE_X+1]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
							}
						}
					}
					if((i/MAZESIZE_X)>0) { //下にマスがある時
						if((white_data[i-MAZESIZE_X]&0x2)!=0x2) { //下のマスと繋がっていたら
							if((white_data[i-MAZESIZE_X]>>8)>j+1) {//現在のマスの下マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i-MAZESIZE_X] = (white_data[i-MAZESIZE_X]&(((j+1)<<8)|0x0F));
								if((i%MAZESIZE_X)<MAZESIZE_X-1) {
									if((white_data[i-MAZESIZE_X]&0x1)!=0x1) {
										if((white_data[i+1-MAZESIZE_X]>>8)>j+2) { //現在のマスの2つ下マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i+1-MAZESIZE_X] = (white_data[i+1-MAZESIZE_X]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
								if((i%MAZESIZE_X)>0) {
									if((white_data[i-MAZESIZE_X-1]&0x1)!=0x1) {
										if((white_data[i-1-MAZESIZE_X]>>8)>j+2) { //現在のマスの2つ下マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i-1-MAZESIZE_X] = (white_data[i-1-MAZESIZE_X]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
							}
						}
					}
					if((i%MAZESIZE_X)>0) { //左にマスがある場合
						if((white_data[i-1]&0x1)!=0x1) { //左のマスと繋がっていたら
							if((white_data[i-1]>>8)>j+1) {//現在のマスの左マスの歩数が現在更新中の歩数+1よりも歩数が大きければ
								white_data[i-1] = (white_data[i-1]&(((j+1)<<8)|0x0F));
								if((i/MAZESIZE_X)>0) {
									if((white_data[i-1-MAZESIZE_X]&0x2)!=0x2) {
										if((white_data[i-1-MAZESIZE_X]>>8)>j+2) { //現在のマスの2つ左マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i-1-MAZESIZE_X] = (white_data[i-1-MAZESIZE_X]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
								if((i/MAZESIZE_X)<MAZESIZE_Y-1) {
									if((white_data[i-1]&0x2)!=0x2) {
										if((white_data[i-1+MAZESIZE_X]>>8)>j+2) { //現在のマスの2つ左マスの歩数が現在更新中の歩数+2よりも歩数が大きければ
											white_data[i-1+MAZESIZE_X] = (white_data[i-1+MAZESIZE_X]&(((MAP_WEIGHT)<<4)|0xFF0F)); //重みを追加
										}
									}
								}
							}
						}
					}
				}
			}
			tail = 0;
			for(k = 0; k < MAZESIZE_X*MAZESIZE_Y; k++) { //現在のターゲット歩数より大きいマスが見つかった場合未更新
				if(j<(white_data[k]>>8)) {
					tail = 1;
				}
			}
			j++;
		}
		i = x + y * MAZESIZE_X;
		while((white_data[i]>>8) != 0) {
			//現在の位置からゴールまでのマスまで予想方向を書き込む
			temp = (white_data[i]>>8)+((white_data[i]&0xF0)>>4);
			if((i/MAZESIZE_X) < MAZESIZE_Y-1) { //上のマスの歩数が現在のマスの歩数より小さい場合上の方向のフラグを立てる
				if((white_data[i]&0x02) != 0x02) {
					if((i/MAZESIZE_X)-buffy != 1) {//曲がった時に重みを追加する
						if((white_data[i+MAZESIZE_X]>>8)+((white_data[i+MAZESIZE_X]&0xF0)>>4) < temp) {
							dire_solve[i] = (0x80);
							temp = (white_data[i+MAZESIZE_X]>>8)+((white_data[i+MAZESIZE_X]&0xF0)>>4);
						}
						else if((white_data[i+MAZESIZE_X]>>8)+((white_data[i+MAZESIZE_X]&0xF0)>>4) == temp) {
							dire_solve[i] = dire_solve[i]|0x80;
						}
					}
					else{
						if((white_data[i+MAZESIZE_X]>>8) < temp) {
							dire_solve[i] = (0x80);
							temp = (white_data[i+MAZESIZE_X]>>8);
						}
						else if((white_data[i+MAZESIZE_X]>>8) == temp) {
							dire_solve[i] = dire_solve[i]|0x80;
						}
					}
				}
			}
			if((i/MAZESIZE_X) > 0) {
				if((white_data[i-MAZESIZE_X]&0x02) != 0x02) {
					if((i/MAZESIZE_X)-buffy != -1) {
						if((white_data[i-MAZESIZE_X]>>8)+((white_data[i-MAZESIZE_X]&0xF0)>>4) < temp) { //下のマスの歩数がさっきの保存した歩数より小さい場合、下のフラグを立ててそれ以外のフラグを消す
							dire_solve[i] = (0x40);
							temp = (white_data[i-MAZESIZE_X]>>8)+((white_data[i-MAZESIZE_X]&0xF0)>>4);
						}
						else if((white_data[i-MAZESIZE_X]>>8)+((white_data[i-MAZESIZE_X]&0xF0)>>4) == temp) {
							dire_solve[i] = dire_solve[i]|0x40;
						}
					}
					else{
						if((white_data[i-MAZESIZE_X]>>8) < temp) { //下のマスの歩数がさっきの保存した歩数より小さい場合、下のフラグを立ててそれ以外のフラグを消す
							dire_solve[i] = (0x40);
							temp = (white_data[i-MAZESIZE_X]>>8);
						}
						else if((white_data[i-MAZESIZE_X]>>8) == temp) {
							dire_solve[i] = dire_solve[i]|0x40;
						}
					}
				}
			}
			if((i%MAZESIZE_X) > 0) {
				if((white_data[i-1]&0x01) != 0x01) {
					if((i%MAZESIZE_X)-buffx != -1) {
						if((white_data[i-1]>>8)+((white_data[i-1]&0xF0)>>4) < temp) { //左のマスの歩数がさっきの保存した歩数より小さい場合、左のフラグを立ててそれ以外のフラグを消す
							dire_solve[i] = (0x20);
							temp = (white_data[i-1]>>8)+((white_data[i-1]&0xF0)>>4);
						}
						else if((white_data[i-1]>>8)+((white_data[i-1]&0xF0)>>4) == temp) {
							dire_solve[i] = dire_solve[i]|0x20;
						}
					}
					else{
						if((white_data[i-1]>>8) < temp) { //左のマスの歩数がさっきの保存した歩数より小さい場合、左のフラグを立ててそれ以外のフラグを消す
							dire_solve[i] = (0x20);
							temp = (white_data[i-1]>>8);
						}
						else if((white_data[i-1]>>8) == temp) {
							dire_solve[i] = dire_solve[i]|0x20;
						}
					}
				}
			}
			if((i%MAZESIZE_X) < MAZESIZE_X-1) {
				if((white_data[i]&0x01) != 0x01) {
					if((i%MAZESIZE_X)-buffx != 1) {
						if((white_data[i+1]>>8)+((white_data[i+1]&0xF0)>>4) < temp) { //右のマスの歩数がさっきの保存した歩数より小さい場合、右のフラグを立ててそれ以外のフラグを消す
							dire_solve[i] = (0x10);
							temp = (white_data[i+1]>>8)+((white_data[i+1]&0xF0)>>4);
						}
						else if((white_data[i+1]>>8)+((white_data[i+1]&0xF0)>>4) == temp) {
							dire_solve[i] = dire_solve[i]|0x10;
						}
					}
					else{
						if((white_data[i+1]>>8) < temp) { //右のマスの歩数がさっきの保存した歩数より小さい場合、右のフラグを立ててそれ以外のフラグを消す
							dire_solve[i] = (0x10);
							temp = (white_data[i+1]>>8);
						}
						else if((white_data[i+1]>>8) == temp) {
							dire_solve[i] = dire_solve[i]|0x10;
						}
					}
				}
			}
			buffx =i % MAZESIZE_X;
			buffy =i / MAZESIZE_X;
			if((dire_solve[i]&0x80) == 0x80) { //これまでで残っている方向のフラグを立てる
				dire_solve[i] = (dire_solve[i]&0x8F);
			}
			else if((dire_solve[i]&0x40) == 0x40) {
				dire_solve[i] = (dire_solve[i]&0x4F);
			}
			else if((dire_solve[i]&0x20) == 0x20) {
				dire_solve[i] = (dire_solve[i]&0x2F);
			}
			else if((dire_solve[i]&0x10) == 0x10) {
				dire_solve[i] = (dire_solve[i]&0x1F);
			}

			if((dire_solve[i]&0x80) == 0x80) {
				i = i + MAZESIZE_X;
			}
			else if((dire_solve[i]&0x40) == 0x40) {
				i = i - MAZESIZE_X;
			}
			else if((dire_solve[i]&0x20) == 0x20) {
				i = i - 1;
			}
			else if((dire_solve[i]&0x10) == 0x10) {
				i = i + 1;
			}
		}
	}
}
