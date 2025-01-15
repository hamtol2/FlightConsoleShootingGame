#include "TestLevel.h"
#include "Engine/Engine.h"
#include "Actor/Player.h"
#include "Actor/PlayerBullet.h"
#include "Actor/Enemy.h"

#include <Windows.h>

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

	// ���� ���.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Color::Green);
	Engine::Get().SetCursorPosition(0, Engine::Get().ScreenSize().y + 1);
	Log("Score: %d", score);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Color::White);

	// �� ����.
	SpawnEnemy(deltaTime);

	// �÷��̾� ź��� ���� �浹 ó��.
	ProcessCollisionPlayerBulletAndEnemy();
}

void TestLevel::SpawnEnemy(float deltaTime)
{
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
	AddActor(new Enemy("[[0]]", Random(1, 10)));
}

void TestLevel::ProcessCollisionPlayerBulletAndEnemy()
{
	// ź�� �� �� ĳ���� �迭 ����.
	List<PlayerBullet*> bullets;
	List<Enemy*> enemies;

	// ������ ��ġ�� ���͸� ��ȸ�ϸ鼭 ����Ʈ ä���.
	for (Actor* actor : actors)
	{
		// ź������ ����ȯ �� Ȯ���ؼ� ����Ʈ ä���.
		PlayerBullet* bullet = actor->As<PlayerBullet>();
		if (bullet)
		{
			bullets.PushBack(bullet);
			continue;
		}

		// ������ ����ȯ �� Ȯ���ؼ� ����Ʈ ä���.
		Enemy* enemy = actor->As<Enemy>();
		if (enemy)
		{
			enemies.PushBack(enemy);
		}
	}

	// ���� ó��.
	if (bullets.Size() == 0 || enemies.Size() == 0)
	{
		return;
	}

	// �� �迭�� ��ȸ�ϸ鼭 �浹 ó��.
	for (PlayerBullet* bullet : bullets)
	{
		for (Enemy* enemy : enemies)
		{
			// ���� ��Ȱ��ȭ ���¶�� �ǳʶٱ�.
			if (!enemy->IsAcive())
			{
				continue;
			}

			// �浹 ó��.
			if (enemy->Intersect(*bullet))
			{
				// �浹������ �� ����.
				enemy->Destroy();

				// ź�൵ ����.
				bullet->Destroy();

				// ���� �߰�.
				score += 100;
			}
		}
	}
}