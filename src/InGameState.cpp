#include "InGameState.h"

InGameState::InGameState(sf::RenderWindow* window) : State(window) 
{
    this->font = new sf::Font();
    this->font->loadFromFile("./resources/font/led_digital_7.ttf");

    //Score System 초기화
    this->scr_system = new ScoreSystem();

    // 공 초기화
    this->ball = new Ball(20.f, 5.f, 1280, 720, scr_system);
    
    //벽돌 초기화
    int rows = 8;
    int bricks_per_row = 3;
    float brick_x = 30.0f;
    float brick_y = 90.0f;
    float brick_itv = 10.0f;
    float brick_row_gap = 6.0f * brick_x;
    float total_height = rows * brick_y + (rows - 1) * brick_itv;
    float start_y = (720 - total_height) / 2;

    for (int row = 0; row < rows; row++) {
        for (int i = 0; i < bricks_per_row; i++) {
            float x = (1280 - (bricks_per_row * (brick_x + brick_row_gap) - brick_row_gap)) / 2 + i * (brick_x + brick_row_gap);
            float y = start_y + row * (brick_y + brick_itv);
            bricks.push_back(new Brick(brick_x, brick_y, sf::Vector2f(x, y)));
        }
    }


	// GameObjects 초기화
    this->players.first = new Player(1);
    this->players.second = new Player(2);

    //스코어 출력을 위한 text 초기화
    this->InitTexts();
}

InGameState::~InGameState() 
{
    delete this->font;
    delete this->ball;

    for (auto& brick : bricks) {
        delete brick;
    }
}

void InGameState::InitTexts()
{
    // 게임 제목 텍스트 설정
    // 순서대로 폰트, 글자 크기, 색상, 메시지, 위치
    score_text.setFont(*(this->font));
    score_p1.setFont(*(this->font));
    score_p2.setFont(*(this->font));


    score_text.setCharacterSize(88);
    score_p1.setCharacterSize(88);
    score_p2.setCharacterSize(88);


    score_text.setFillColor(sf::Color(255, 255, 255, 50));
    //스코어 4점 이상 부터 빨간색으로 
    if (this->scr_system->GetScore(1) >= 4)
    {
        score_p1.setFillColor(sf::Color::Red);
    }
    else
    {
        score_p1.setFillColor(sf::Color(255, 255, 255, 50));
    }

    if (this->scr_system->GetScore(2) >= 4)
    {
        score_p2.setFillColor(sf::Color::Red);
    }
    else
    {
        score_p2.setFillColor(sf::Color(255, 255, 255, 50));
    }

    score_p1.setString(std::to_string(this->scr_system->GetScore(1)));
    score_text.setString(" : ");
    score_p2.setString(std::to_string(this->scr_system->GetScore(2)));

    // **포지션 수정**
    float total_width = score_p1.getLocalBounds().width + score_text.getLocalBounds().width + score_p2.getLocalBounds().width;
    float start_x = (1280 - total_width) / 2;
    float spacing = 40.0f;

    score_p1.setPosition(start_x- spacing, 150);
    score_text.setPosition(start_x + score_p1.getLocalBounds().width , 150);
    score_p2.setPosition(start_x + score_p1.getLocalBounds().width + score_text.getLocalBounds().width + spacing, 150);
}


int InGameState::EndState() 
{
    return this->winner_num;
}

void InGameState::UpdateInput(const float& dt) 
{
    this->players.first->UpdateInput(dt);
    this->players.second->UpdateInput(dt);
}

void InGameState::Update(const float& dt) 
{
    // 게임 종료 확인
    this->CheckForQuit();

    // 공 움직임 처리
    this->ball->Move();
    this->ball->CheckCollisionWithWall();

    //벽돌 구현부
    this->ball->CheckCollisionWithBrick(bricks);
        
    // 플레이어
	this->players.first->Update(dt);
    this->players.second->Update(dt);

	// 공의 global bounds와 player의 충돌 확인
	if (this->ball->GetShape().getGlobalBounds().intersects(this->players.first->GetDrawable()->getGlobalBounds()))
	{
        this->players.first->OnCollision(ball);
	}
    else
	{
        this->players.first->ClearCollisionTrigger();
	}

    if (this->ball->GetShape().getGlobalBounds().intersects(this->players.second->GetDrawable()->getGlobalBounds()))
    {
		this->players.second->OnCollision(ball);
	}
    else
    {
		this->players.second->ClearCollisionTrigger();
    }

    //스코어 업데이트
    this->InitTexts();
}

void InGameState::Render(sf::RenderTarget* target) 
{
    // 출력 대상 미지정 시 현재 화면으로 설정
    if (!target)
        target = this->window_;

    // target->draw(대상); 으로 렌더링하길 권장합니다.

    // 화면에 공 그리기
    target->draw(this->ball->GetShape());

    for (auto& brick : bricks) {
        target->draw(brick->GetShape());
    }

    // 플레이어 그리기
    target->draw(*this->players.first->GetDrawable());
    target->draw(*this->players.second->GetDrawable());

    // 스코어 출력하기
    target->draw(score_text);
    target->draw(score_p1);
    target->draw(score_p2);
}

void InGameState::CheckForQuit()
{
    //base class
    State::CheckForQuit();

    //득점 조건 달성 시 게임 종료
    int winner;
    if(scr_system->IsGameFinished(winner))
    {
        this->winner_num = winner;
        this->quit_ = true;
    }
}