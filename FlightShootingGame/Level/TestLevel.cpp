#include "TestLevel.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include "Actor/PlayerBullet.h"
#include "Actor/Enemy.h"
#include "Actor/EnemyBullet.h"
#include "Container/List.h"
#include <Windows.h>

// �� ĳ���� ����.
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

	// �� ����.
	SpawnEnemy(deltaTime);

	// �÷��̾� ź��� ���� �浹 ó��.
	ProcessCollisionPlayerBulletAndEnemy();

	// �÷��̾�� �� ź���� �浹 ó��.
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

	// ���� ���.
	char buffer[256];
	snprintf(buffer, 256, "Score: %d", score);
	Engine::Get().Draw(Vector2(0, Engine::Get().ScreenSize().y - 1), buffer);
}

void TestLevel::SpawnEnemy(float deltaTime)
{
	// ���� ó��.
	if (deltaTime > 1.0f)
	{
		return;
	}

	// ESC Ű�� ����.
	if (Engine::Get().GetKeyDown(VK_ESCAPE))
	{
		Engine::Get().QuitGame();
	}

	// �� ����.
	static float elapsedTime = 0.0f;
	static float spawnTime = RandomPercent(1.0f, 3.0f);

	// Ÿ�̸�.
	elapsedTime += deltaTime;
	if (elapsedTime < spawnTime)
	{
		return;
	}

	// Ÿ�̸� ����.
	elapsedTime = 0.0f;
	spawnTime = RandomPercent(1.0f, 3.0f);

	// �� ����.
	static int length = sizeof(enemyType) / sizeof(enemyType[0]);
	int index = Random(0, length - 1);

	//AddActor(new Enemy("[[0]]", Random(1, 10)));
	AddActor(new Enemy(enemyType[index], Random(1, 10)));
}

void TestLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// ź�� �� �� ĳ���� �迭 ����.
	List<PlayerBullet*> bullets;
	List<Enemy*> enemies;

	// ���� ����� ��ȸ�ϸ鼭 ź��� �� ĳ���͸� ã�� �迭�� �߰�.
	for (Actor* actor : actors)
	{
		// �÷��̾� ź������ ����ȯ �����ϸ� ��Ͽ� �߰�.
		PlayerBullet* bullet = actor->As<PlayerBullet>();
		if (bullet)
		{
			bullets.PushBack(bullet);
			continue;
		}

		// ������ ����ȯ �����ϸ� ��Ͽ� �߰�.
		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			enemies.PushBack(enemy);
		}
	}

	// ź���̳� ���� ������ �浹 ó������ �ʰ� ����.
	if (bullets.Size() == 0 || enemies.Size() == 0)
	{
		return;
	}

	// �� �迭�� ��ȸ�ϸ鼭 �浹 ó��.
	for (PlayerBullet* bullet : bullets)
	{
		for (Enemy* enemy : enemies)
		{
			// ���� ��Ȱ��ȭ �����̸� �ǳʶٱ�.
			if (!enemy->IsAcive())
			{
				continue;
			}

			// ���� ź���� �浹�ߴ��� Ȯ��.
			if (enemy->Intersect(*bullet))
			{
				enemy->Destroy();

				// ���� ȹ��.
				score += 1;
			}
		}
	}
}

void TestLevel::ProcessCollisionPlayerAndEnemyBullet()
{
	// ź�� �� �� ĳ���� �迭 ����.
	List<EnemyBullet*> bullets;
	Player* player = nullptr;

	// ���� ����� ��ȸ�ϸ鼭 ź��� �� ĳ���͸� ã�� �迭�� �߰�.
	for (Actor* actor : actors)
	{
		// �÷��̾ �������� �ʾ����� �÷��̾�� ����ȯ.
		if (!player)
		{
			player = actor->As<Player>();
			continue;
		}

		// ������ ����ȯ �����ϸ� ��Ͽ� �߰�.
		EnemyBullet* bullet = actor->As<EnemyBullet>();
		if (bullet)
		{
			bullets.PushBack(bullet);
		}
	}

	// �÷��̾ ��ã�Ұų� �� ź���� ������ �浹 ó������ �ʰ� ����.
	if (player == nullptr || bullets.Size() == 0)
	{
		return;
	}

	// �� �迭�� ��ȸ�ϸ鼭 �浹 ó��.
	for (EnemyBullet* bullet : bullets)
	{
		// �÷��̾ ��Ȱ��ȭ �����̸� �ǳʶٱ�.
		if (!player->IsAcive())
		{
			continue;
		}

		// ���� ź���� �浹�ߴ��� Ȯ��.
		if (player->Intersect(*bullet))
		{
			isPlayerDead = true;
			playerDeadPosition = Vector2(player->Position().x + player->Width() / 2, player->Position().y);
			player->Destroy();
		}
	}
}