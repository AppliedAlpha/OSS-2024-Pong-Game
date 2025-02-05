#pragma once

#include "header/stdafx.h";
#include <vector>

/// <summary>
/// Pong Game 속 두 플레이어의 점수를 계산하는 시스템입니다.
/// 한 쪽이 winning Score에 도달할 경우, State에 Exit함수를 호출하여 Quit하게 됩니다.
/// </summary>
class ScoreSystem 
{
private:
	std::pair<int, int> scores_;
	const int kWinningScore = 5;

//	methods
	bool CheckGameFinish() const;
	void ResetScore();
public:
	ScoreSystem();
	~ScoreSystem();
	
	void AddScore(int player_number, int score = 1);
	int GetScore(int player_number) const;
	bool IsGameFinished(int& winner);
};