#include "TestLevel.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include "Actor/PlayerBullet.h"
#include "Actor/Enemy.h"
#include "Actor/EnemyBullet.h"
#include "Container/List.h"
#include <Windows.h>

// 적 캐릭터 종류.
static const char* enemyType[] =
{
	";(^);",
	"zZTZz",
	"oO&Oo",
	"<=-=>",
	")~O~(",
	"[[0]]"
};

TestLevel::TestLevel()
{
	AddActor(new Player("<A>"));
}

TestLevel::~TestLevel()
{
}

void TestLevel::Update(float deltaTime)
{
	Super::Update(deltaTime);

	// 적 생성.
	SpawnEnemy(deltaTime);

	// 플레이어 탄약과 적의 충돌 처리.
	ProcessCollisionPlayerBulletAndEnemy();

	// 플레이어와 적 탄약의 충돌 처리.
	ProcessCollisionPlayerAndEnemyBullet();
}

void TestLevel::Draw()
{
	Super::Draw();

	if (isPlayerDead)
	{
		int y = Engine::Get().ScreenSize().y;
		Engine::Get().Draw(Vector2(playerDeadPosition.x, playerDeadPosition.y - 1), "   .   ");
		Engine::Get().Draw(Vector2(playerDeadPosition.x, playerDeadPosition.y), " .  .  .");
		Engine::Get().Draw(Vector2(playerDeadPosition.x, playerDeadPosition.y + 1), "..:V:..");
		Engine::Get().Draw(Vector2(playerDeadPosition.x, playerDeadPosition.y + 2), "Game Over!");
		Engine::Get().PresentImmediately();

		Sleep(3000);

		Engine::Get().QuitGame();
	}

	// 점수 출력.
	char buffer[256];
	snprintf(buffer, 256, "Score: %d", score);
	Engine::Get().Draw(Vector2(0, Engine::Get().ScreenSize().y - 1), buffer);
}

void TestLevel::SpawnEnemy(float deltaTime)
{
	// 예외 처리.
	if (deltaTime > 1.0f)
	{
		return;
	}

	// ESC 키로 종료.
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().QuitGame();
	}

	// 적 생성.
	static float elapsedTime = 0.0f;
	static float spawnTime = RandomPercent(1.0f, 3.0f);

	// 타이머.
	elapsedTime += deltaTime;
	if (elapsedTime < spawnTime)
	{
		return;
	}

	// 타이머 리셋.
	elapsedTime = 0.0f;
	spawnTime = RandomPercent(1.0f, 3.0f);

	// 적 생성.
	static int length = sizeof(enemyType) / sizeof(enemyType[0]);
	int index = Random(0, length - 1);

	//AddActor(new Enemy("[[0]]", Random(1, 10)));
	AddActor(new Enemy(enemyType[index], Random(1, 10)));
}

void TestLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// 탄약 및 적 캐릭터 배열 선언.
	List<PlayerBullet*> bullets;
	List<Enemy*> enemies;

	// 액터 목록을 순회하면서 탄약과 적 캐릭터를 찾아 배열에 추가.
	for (Actor* actor : actors)
	{
		// 플레이어 탄약으로 형변환 성공하면 목록에 추가.
		PlayerBullet* bullet = actor->As<PlayerBullet>();
		if (bullet)
		{
			bullets.PushBack(bullet);
			continue;
		}

		// 적으로 형변환 성공하면 목록에 추가.
		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			enemies.PushBack(enemy);
		}
	}

	// 탄약이나 적이 없으면 충돌 처리하지 않고 종료.
	if (bullets.Size() == 0 || enemies.Size() == 0)
	{
		return;
	}

	// 두 배열을 순회하면서 충돌 처리.
	for (PlayerBullet* bullet : bullets)
	{
		for (Enemy* enemy : enemies)
		{
			// 적이 비활성화 상태이면 건너뛰기.
			if (!enemy->IsAcive())
			{
				continue;
			}

			// 적과 탄약이 충돌했는지 확인.
			if (enemy->Intersect(*bullet))
			{
				enemy->Destroy();

				// 점수 획득.
				score += 1;
			}
		}
	}
}

void TestLevel::ProcessCollisionPlayerAndEnemyBullet()
{
	// 탄약 및 적 캐릭터 배열 선언.
	List<EnemyBullet*> bullets;
	Player* player = nullptr;

	// 액터 목록을 순회하면서 탄약과 적 캐릭터를 찾아 배열에 추가.
	for (Actor* actor : actors)
	{
		// 플레이어가 설정되지 않았으면 플레이어로 형변환.
		if (!player)
		{
			player = actor->As<Player>();
			continue;
		}

		// 적으로 형변환 성공하면 목록에 추가.
		EnemyBullet* bullet = actor->As<EnemyBullet>();
		if (bullet)
		{
			bullets.PushBack(bullet);
		}
	}

	// 플레이어를 못찾았거나 적 탄약이 없으면 충돌 처리하지 않고 종료.
	if (player == nullptr || bullets.Size() == 0)
	{
		return;
	}

	// 두 배열을 순회하면서 충돌 처리.
	for (EnemyBullet* bullet : bullets)
	{
		// 플레이어가 비활성화 상태이면 건너뛰기.
		if (!player->IsAcive())
		{
			continue;
		}

		// 적과 탄약이 충돌했는지 확인.
		if (player->Intersect(*bullet))
		{
			isPlayerDead = true;
			playerDeadPosition = Vector2(player->Position().x + player->Width() / 2, player->Position().y);
			player->Destroy();
		}
	}
}