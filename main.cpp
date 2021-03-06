/*
		-----------------------------------------------------------------------------
						A S T E R O I D S     I N     4     H O U R S
		-----------------------------------------------------------------------------
															 		 by Minho Menezes

		O c�digo abaixo corresponde ao resultado do Workshop "ASTEROIDS EM 4 HORAS: 
		INTRODU��O � PROGRAMA��O DE GAMES COM C++ E SFML".

		O intuito do Workshop foi apresentar, aos participantes, diversas t�cnicas e
		abordagens comuns � Programa��o de Games, apresentado-os a conceitos essenciais
		e familiarizando-os com o ambiente de conceber um jogo atrav�s da Programa��o.

		IMPORTANTE:
		Para que o c�digo compile, e possa ser executado, � necess�rio criar um projeto
		com a biblioteca SFML configurada. Diversos tutoriais para instala��o do SFML
		em diferentes IDE's podem ser encontrados pela internet; mais especificamente
		no pr�prio site da biblioteca: http://www.sfml-dev.org/

		Para mais informa��es, d�vidas ou curiosidades, me contate: 
		E-mail: minhotmog@gmail.com
		
		E divirta-se! :)
*/

// ----- Bibliotecas -----
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <vector>
#include <time.h>
// ------------------------

// Programa Principal
int main()
{
	// --------------------------- Declara��es ---------------------------
	// Janela
	// Estrutura do Construtor: ( [Dimens�o], [T�tulo], [Estilo] )
	sf::RenderWindow window(sf::VideoMode(800, 600), "Asteroids 4 Horas", sf::Style::Close);		// Janela do Jogo

	// EXTRA #01: MENU INICIAL
	// Menu Inicial
	sf::Sprite startMenu;							// Objeto do tipo "sf::Sprite" que representa a Imagem de Fundo do Menu Inicial
	sf::Texture startMenuImg;						// Textura da Imagem de Fundo a ser carregada em "startMenu"
	bool isStart = true;							// Verifica se o MenuInicial est� ou n�o ativo. Inicialmente, est� ativo.

	// EXTRA #02: HUD (Life e Score)
	sf::Sprite shipsAvailable[3];					// Representa o "life" da Nave, indicando na HUD quantas ainda est�o dispon�veis
	sf::Text scoreText;								// Texto a ser impresso na Tela para indicar o score atual
	sf::Font scoreTextFont;							// Fonte necess�ria para a Renderiza��o do Texto
	int score = 0;									// Vari�vel para armazenar o valor do Score j� obtido

	// EXTRA #03: GAME OVER
	sf::Sprite gameOverBG;							// Objeto do tipo "sf::Sprite" que representa a Imagem de Fundo da tela de Game Over
	sf::Texture gameOverBGTxt;						// Textura da Imagem de Fundo a ser carregada em "gameOverBG"
	bool isOver = false;							// Verifica se o Game Over est� ou n�o ativo. Inicialmente, est� inativo.
	sf::Clock restartTimer;							// Timer para cronometrar o tempo para Resetar o Jogo
	sf::Text timerText;								// Texto para exibir o Timer de restart do Jogo. Carregar� a fonte de "scoreTextFont".
	
	// EXTRA #04: MUSIC
	sf::Sound bgMusic;								// Objeto de "sf::Sound" que representa a M�sica de Fundo do Game
	sf::SoundBuffer bgmBuffer;						// Buffer que armazena a faixa de �udio a ser executada por "bgMusic"

	// Nave
	sf::Sprite ship;								// Objeto do tipo "sf::Sprite" que representa o Sprite
	sf::Texture shipTxt;							// Objeto do tipo "sf::Texture" que implementa a Textura do Sprite
	sf::Vector2f sVelocity(0, 0);					// Vetor Bi-dimensional do tipo Float (Vector-2-F) para manipular a Velocidade
	float sAngle = 0;								// �ngulo de Rota��o da Nave. Inicialmente 0 pois Nave aponta para a Direita.
	int sLife = 3;									// Vari�vel para indicar o "life" da Nave. Inicializada com o valor 3.
	sf::Clock bTrigger;								// Timer para controlar a frequ�ncia de disparos de Proj�teis
	sf::Clock sSpawn;								// Timer para controlar a frequ�ncia de Colis�o da Nave com os Asteroids.

	// Proj�til
	std::vector<sf::RectangleShape> listBullets;	// Lista Encadeada de Proj�teis do tipo "sf::RectangleShape"
	std::vector<sf::Vector2f> bVelocity;			// Lista Encadeada de Velocidade na forma de Vetor-2-Float (Espelhada a "listBullets")

	// Asteroids
	std::vector<sf::CircleShape> listAsteroids;		// Lista Encadeada de Asteroids do tipo "sf::CircleShape"
	std::vector<sf::Vector2f> aVelocity;			// Lista Encadeada de Velocidade na forma de Vetor-2-Float (Espelhada a "listAsteroids")
	
	// Moldes (ou Modelos)
	sf::CircleShape aMolde[3];						// Modelo para 3 Tipos de Asteroids
	sf::RectangleShape bMolde;						// Modelo uniforme para os Proj�teis
	

	// ------------------------- Inicializa��es -------------------------
	// Inicializa��o da Janela
	window.setFramerateLimit(60);					// Limita o FPS em um teto de 60FPS, controlando a performance do Game.

	// Inicializa��es da Nave
	shipTxt.loadFromFile("res/ship.png");			// Carrega a imagem que est� em "res/ship.png" para a textura "shipTxt".
	ship.setTexture(shipTxt);						// Seleciona "shipTxt" como Textura do sprite "ship"
	
	// EXTRA #01: MENU INICIAL
	// Inicializa��o do Menu Inicial
	startMenuImg.loadFromFile("res/startScreen.png");
	startMenu.setTexture(startMenuImg);

	// EXTRA #02: HUD
	// Inicializa��o do HUD (Life)
	for (int i = 0; i < 3; i++) {
		shipsAvailable[i].setTexture(shipTxt);	// Carrega as Texturas nos tr�s Sprites com base na Textura de "ship"
		
		// Iremos posicionar cada Nave da seguinte forma:
		// Eixo X: Extremidade da esquerda somado de 15 e somado pelo tamanho do Sprite multiplicado pelo �ndice (posi��o)
		// Eixo Y: fixo na Extremidade superior somado de 50.
		shipsAvailable[i].setPosition(15 + (i * shipsAvailable[i].getLocalBounds().width), 80);
		shipsAvailable[i].setRotation(270);
	}
	
	// Inicializa��o do HUD (Score)
	scoreTextFont.loadFromFile("res/PressStart2P.ttf");
	scoreText.setFont(scoreTextFont);
	scoreText.setCharacterSize(14);
	scoreText.setPosition(25, 25);
	scoreText.setString("00");

	// EXTRA #03: GAME OVER
	// Inicializa��o do Menu Inicial
	gameOverBGTxt.loadFromFile("res/gameOverScreen.png");
	gameOverBG.setTexture(gameOverBGTxt);

	timerText.setFont(scoreTextFont);
	timerText.setCharacterSize(100);
	timerText.setPosition(350, 350);

	// EXTRA #04: MUSIC
	bgmBuffer.loadFromFile("res/asteroidsBGM.ogg");
	bgMusic.setBuffer(bgmBuffer);

	// Na linha a seguir, modificamos o Ponto de Origem do Sprite da Nave para sua posi��o central
	// Para isso, n�s usamos o m�todo ".getLocalBounds()" para recebermos o Ret�ngulo que contorna o Sprite, e,
	// ent�o, acessamos os atributos "width" e "height" desse Ret�ngulo para a Dimens�o do Sprite.
	ship.setOrigin(ship.getLocalBounds().width / 2, ship.getLocalBounds().height / 2);

	// Nessa linha modificamos a posi��o do Ponto de Origem do Sprite para o centro da Janela
	ship.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	// Inicializa��o do 3 Moldes dos Asteroids
	for (int i = 0; i < 3; i++)
	{
		aMolde[i].setFillColor(sf::Color::Transparent);			// Selecionamos a cor de preenchimento como "Transparente"
		aMolde[i].setPointCount(6);								// Indicamos que o Formato ter� apenas 6 Arestas
		aMolde[i].setOutlineThickness(1 + i);					// Damos um valor de "1 + i" para a grossura da Linha de Contorno
		aMolde[i].setRadius(10 * (1 + i));						// Damos um valor de "10 * (1 + i)" para o Raio do Molde
		aMolde[i].setOrigin(5 * (1 + i), 5 * (1 + i));			// Modificamos o Ponto de Origem para o centro da Figura
	}

	// Inicializa��o do Molde dos Proj�teis
	bMolde.setFillColor(sf::Color::White);						// Selecionamos a cor de preenchimento como "Branco"
	bMolde.setSize(sf::Vector2f(10, 4));						// Informamos as Dimens�es do Proj�til (10x4 pixels)
	bMolde.setOrigin(5, 2);										// Modificamos o Ponto de Origem para o centro da Figura

	// Inicializa��o do Seed para o Gerador Aleat�rio ( rand() )
	srand(time(NULL));

	// Game Loop ____________________________________________________
	while (window.isOpen())		// L�-se: "Equanto a Janela est� aberta".
	{
		// -------------------------- Tratamento de Eventos --------------------------
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// EXTRA #01: MENU INICIAL
		// Os Menu's exigem "Loops", assim como o Game Loop. Mas, para mantermos o n�vel
		// iremos "economizar" esse Loop e incluir o Menu Loop dentro do Game Loop usando
		// o Booleano "isStart" para separarmos a L�gica do Game da L�gica do Menu. Veja:
		
		// Menu Inicial Loop
		if (isStart)
		{
			// ----------------------- Input Handling -----------------------
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				isStart = false;		// Desligamos o Menu 

				// EXTRA #04: MUSICA
				bgMusic.play();			// Ligamos a M�sica de Fundo
			}

			// ----------------------- Tratamento de Renderiza��es -----------------------
			window.clear();
			window.draw(startMenu);
			window.display();
			
		}
		// EXTRA #02: GAME OVER
		// Game Over Loop
		else if (isOver)
		{
			// ----------------------- Processamento -----------------------
			// Nesse "loop" do Game Over, n�s iremos manter um Timer rodando, sempre atualizando Texto
			// de Countdown a ser exibido na Tela. Quando esse tempo se esgotar (ou seja, "restartTimer"
			// tiver um tempo maior que 10 segundos) n�s iremos desligar a tela de Game Over, e re-ligar
			// o Menu Inicial.
			// Vale lembrar que o "restartTimer" tem que ser "resatado" assim que o Jogador morrer, quando
			// "sLife" for igual a Zero, l� no Game Loop.
			if (restartTimer.getElapsedTime().asSeconds() <= 10)
				// ATEN��O: Se sua IDE n�o possui o C++11, ele n�o ir� reconhecer a fun��o "to_string()".
				// Leia o "readme.txt" que cont�m o trecho de c�digo que voc� dever� utilizar para a convers�o, nesse caso!
				timerText.setString(std::to_string(9 - (int)restartTimer.getElapsedTime().asSeconds()));
			else {
				isStart = true;
				isOver = false;
			}
		
			// ----------------------- Tratamento de Renderiza��es -----------------------
			window.clear();
			window.draw(gameOverBG);
			window.draw(timerText);
			window.display();

		}
		// Game Loop
		else
		{
			// ----------------------- Input Handling -----------------------
			// Tecla Z (com delay de 0.5segundos)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && bTrigger.getElapsedTime().asMilliseconds() >= 500)
			{
				// Posicionamos e Rotacionamos o Molde do Proj�til de acordo com a Nave
				bMolde.setPosition(ship.getPosition());
				bMolde.setRotation(ship.getRotation());

				// Geramos uma Velocidade baseada na rota��o da Nave (sAngle) no momento do disparo.
				sf::Vector2f tempVel;
				tempVel.x = cos(sAngle * 3.1415 / 180) * 5;
				tempVel.y = sin(sAngle * 3.1415 / 180) * 5;

				// Inserimos o Proj�til e sua Velocidade nas suas respectivas Listas
				listBullets.push_back(bMolde);
				bVelocity.push_back(tempVel);

				// Resetamos o Timer do Delay de 0.5 Segundos
				bTrigger.restart();
			}

			// Tecla "UP"
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				// Modificamos as componentes vetoriais da Velocidade na Nave de acordo com o seu �ngulo de rota��o
				sVelocity.x += cos(sAngle * 3.1415 / 180) * 0.02;
				sVelocity.y += sin(sAngle * 3.1415 / 180) * 0.02;
			}

			// Tecla "Left" e "Right":
			// Diminuimos ou Aumentamos, respectivamente, o valor do �ngulo de Rota��o.
			// Vale lembrar que o valor dos �ngulos, em grau, crescem no sentido Hor�rio!
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				sAngle -= 2;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				sAngle += 2;


			// ----------------------- Processamento -----------------------
			// Corre��o do Valor de �ngulo (Deve permanecer no intervalo 0~360)
			if (sAngle > 360)
				sAngle -= 360;
			else if (sAngle < 0)
				sAngle = +360;

			// Corre��o de Posicionamento da Nave
			// No nosso Algoritmo, n�s verificamos se a posi��o da Nave ultrapassa algum dos
			// quatro limites da Janela, e ent�o o "puxamos" para a outra extremidade oposta da
			// Janela usando o m�todo ".move()".
			if (ship.getPosition().x < 0)
				ship.move(800, 0);
			else if (ship.getPosition().x > window.getSize().x)
				ship.move(-800, 0);

			if (ship.getPosition().y < 0)
				ship.move(0, 600);
			else if (ship.getPosition().y > window.getSize().y)
				ship.move(0, -600);

			// Execu��o da Rota��o e Movimenta��o do Sprite da Nave
			ship.setRotation(sAngle);
			ship.move(sVelocity);

			// Execu��o do Movimenta��o de todos os Proj�teis de "listBullets"
			for (int i = 0; i < listBullets.size(); i++)
				listBullets[i].move(bVelocity[i]);

			// Corre��o de Posicionamento dos Asteroids
			// Segue a mesma l�gica da Corre��o de Posicionamento da Nave
			// Com a diferen�a de que a verifica��o deve ser feita para todos os
			// Asteroids contidos na lista "listAsteroids"
			for (int i = 0; i < listAsteroids.size(); i++)
			{
				if (listAsteroids[i].getPosition().x < 0)
					listAsteroids[i].move(800, 0);
				else if (listAsteroids[i].getPosition().x > window.getSize().x)
					listAsteroids[i].move(-800, 0);

				if (listAsteroids[i].getPosition().y < 0)
					listAsteroids[i].move(0, 600);
				else if (listAsteroids[i].getPosition().y > window.getSize().y)
					listAsteroids[i].move(0, -600);
			}

			// Execu��o do Movimenta��o de todos os Asteroids de "listAsteroids"
			for (int i = 0; i < listAsteroids.size(); i++)
				listAsteroids[i].move(aVelocity[i]);

			// Spawning Asteroids
			// Sempre que a lista de Asteroids estiver vazia, iremos criar novos 4
			// Asteroids, cada qual em um trecho aleat�rio das 4 extremidades da Janela.
			if (listAsteroids.empty())
			{
				for (int i = 0; i < 4; i++)
				{
					// Consideramos os Asteroids pares (0 e 2) como os que aparecem nas extremidades Verticais
					if (i % 2 == 0)
					{
						// Geramos um valor aleat�rio para a posi��o no eixo X e posicionamos o molde de acordo.
						int posX = rand() % 800 + 1; // 0 - 800
						aMolde[2].setPosition(posX, i * 300);
					}
					// Consideramos os Asteroids pares (1 e 3) como os que aparecem nas extremidades Horizontais
					else
					{
						// Geramos um valor aleat�rio para a posi��o no eixo Y e posicionamos o molde de acordo.
						int posY = rand() % 600 + 1; // 0 - 800
						aMolde[2].setPosition((i - 1) * 400, posY);
					}

					// Criamos uma Velocidade e Rota��o para o Asteroid.
					// Para isso, geramos um valor aleat�rio de 0~360 para o seu �ngulo de Rota��o, e
					// calculamos a velocidade com base nisto.
					float tempAngle;
					tempAngle = rand() % 360 + 1;
					aMolde[2].setRotation(tempAngle);

					sf::Vector2f tempVel;
					tempVel.x = cos(tempAngle * 3.1415 / 180) * 1;
					tempVel.y = sin(tempAngle * 3.1415 / 180) * 1;

					// Inserimos o novo Asteroid e sua Velocidade nas suas respectivas Listas
					listAsteroids.push_back(aMolde[2]);
					aVelocity.push_back(tempVel);
				}
			}


			// ----------------------- Tratamento de Colis�o -----------------------
			// Verifica��o e Tratamento de Colis�o para a Colis�o Asteroid-Proj�til
			// N�s verificaremos, para cada Asteroid, se o mesmo colide com qualquer Proj�til
			// de "listBullets". Se sim, n�s iremos apagar o referido Proj�til da Lista,
			// gerar 2 Novos Asteroids (apenas se o referido Asteroid n�o for o menor) e
			// apagar o Asteroid colidido de "listAsteroids".
			for (int i = 0; i < listAsteroids.size(); i++)
			{
				for (int j = 0; j < listBullets.size(); j++)
				{
					// O "if" abaixo VERIFICA a Colis�o
					if (listBullets[j].getGlobalBounds().intersects(listAsteroids[i].getGlobalBounds()))
					{
						// Apagamos o Proj�til da posi��o "j" da lista "listBullets" (e apagamos sua velocidade
						// na lista espelhada "bVelocity").
						listBullets.erase(listBullets.begin() + j);
						bVelocity.erase(bVelocity.begin() + j);

						// Se o Asteroid da posi��o "i" n�o for o Menor (Raio igual a 10)...
						if (listAsteroids[i].getRadius() > 10)
						{
							// Criamos um �ndice para nos referirmos ao �ndice do Molde imediatamente menor
							// ao do Asteroid a ser destru�do. Veja:
							// Asteroid Grande -> Molde[2] -> Raio = 30. (Raio / 10 - 2) == 1 -> Molde[1] < Molde[2]
							// Asteroid M�dio -> Molde[1] -> Raio = 20. (Raio / 10 - 2) == 0 -> Molde[0] < Molde[1]
							int index = listAsteroids[i].getRadius() / 10 - 2;

							// Posicionamos o Molde dos Novos Asteroids para a posi��o do Asteroid Original
							aMolde[index].setPosition(listAsteroids[i].getPosition());

							// Geramos duas velocidades - uma para cada Asteroid que ir� surgir -, onde cada nova
							// velocidade varia 45� do �ngulo do vetor anterior. 
							sf::Vector2f tempVel[2];

							tempVel[0].x = cos((listAsteroids[i].getRotation() + 45) * 3.1415 / 180) * 1;
							tempVel[0].y = sin((listAsteroids[i].getRotation() + 45) * 3.1415 / 180) * 1;

							tempVel[1].x = cos((listAsteroids[i].getRotation() - 45) * 3.1415 / 180) * 1;
							tempVel[1].y = sin((listAsteroids[i].getRotation() - 45) * 3.1415 / 180) * 1;


							// Inserimos duas c�pias iguais do Molde em "listAsteroids", e cada uma das suas 
							// respectivas velocidades na lista "aVelocity"
							listAsteroids.push_back(aMolde[index]); listAsteroids.push_back(aMolde[index]);
							aVelocity.push_back(tempVel[0]); aVelocity.push_back(tempVel[1]);
						}

						// EXTRA #02: HUD
						// Pontua��o no Score
						score += (60 - listAsteroids[i].getRadius());		// Scores de 30 (Grande), 40 (M�dio) e 50 (Pequeno)

						// Tranforma��o do Score em Texto
						// ATEN��O: Se sua IDE n�o possui o C++11, ele n�o ir� reconhecer a fun��o "to_string()".
						// Leia o "readme.txt" que cont�m o trecho de c�digo que voc� dever� utilizar para a convers�o, nesse caso!
						scoreText.setString(std::to_string(score));			

						// Apagamos o Asteroid da posi��o "i" da lista "listAsteroid" (e apagamos sua velocidade
						// na lista espelhada "aVelocity").
						listAsteroids.erase(listAsteroids.begin() + i);
						aVelocity.erase(aVelocity.begin() + i);

						// Como a exclus�o do Asteroid de "listAsteroid" far� com que os �ndices dos elementos
						// posteriores sejam diminuidos no valor de 1 (um), ent�o devemos diminuir o �ndice do
						// "for" para garantirmos que nenhum Asteroid deixar� de ser verificado.
						i--; break;
					}
				}
			}

			// Verifica��o e Tratamento de Colis�o para a Colis�o Asteroid-Nave
			// Essa verifica��o de Colis�o � simpl�ria, e voc� notar� que n�o � muito eficiente
			// devido �s regi�es "transparentes" dos Elementos do Game
			// N�s verificaremos, para cada Asteroid, se o mesmo colide a Nave.
			// Se a Colis�o for verdadeira, ent�o n�s iremos decrementar uma vari�vel "sLife"
			// (que deve ser criada agora). Um detalhe, por�m, � importante: pelo
			// mesmo motivo que verificamos no Disparo de Proj�teis, aqui tamb�m
			// devemos ter um Timer para controlar a Frequ�ncia de Colis�es, se 
			// n�o a Nave receber� 60 Colis�es por segundo! (Criamos o timer "sSpawn").
			// Iremos dizer que a Nave s� poder� colidir com outro Asteroid ap�s 3 segundos.
			for (int i = 0; i < listAsteroids.size(); i++)
			{
				if (listAsteroids[i].getGlobalBounds().intersects(ship.getGlobalBounds()) && sSpawn.getElapsedTime().asSeconds() > 3)
				{
					sLife--;				// Decrementamos o "life" da Nave
					sSpawn.restart();		// Resetamos o Timer da Colis�o

					// EXTRA #03: GAME OVER
					// Verifica��o de Game Over
					if (sLife == 0)
					{
						// Quando a "Nave" morrer, vamos resetar todos os elementos do Game
						isOver = true;

						score = 0;
						sLife = 3;

						listAsteroids.clear();
						aVelocity.clear();

						listBullets.clear();
						bVelocity.clear();


						// EXTRA #03: GAME OVER
						restartTimer.restart();

						// EXTRA #04: MUSICA
						bgMusic.stop();
					}
				}
			}

			// Aqui, ent�o, faremos uma coisa interessante: iremos fazer a Nave "piscar" quando
			// n�o estiver dispon�vel para outra Colis�o, e mant�-la fixa na Posi��o Inicial! 
			// Temos como saber que a Nave est� sem poder Colidir quando o timer "sSpawn" � menor 
			// que 3 Segundos, pois esse � o tempo m�nimo entre Colis�o. Faremos, ent�o, com que 
			// a Nave mude entre Opaco e Transparente de 100 em 100 millisegundos, enquanto o tempo 
			// total for menor que 3 Segundos.
			// Para isso usaremos o m�todo ".setColor()" que existe na classe "sf::Sprite".
			if (sSpawn.getElapsedTime().asSeconds() <= 3)
			{
				// Aqui n�s fazemos a Nave "piscar"
				if (sSpawn.getElapsedTime().asMilliseconds() % 100 < 50)		// � necess�rio aumentarmos a margem de erro por 50millisegundos
				{
					if (ship.getColor().a == 255)
						ship.setColor(sf::Color(ship.getColor().r, ship.getColor().g, ship.getColor().b, 0));		// Mantemos costante os valores de RGB e mudamos somente o Alpha para 0.
					else
						ship.setColor(sf::Color(ship.getColor().r, ship.getColor().g, ship.getColor().b, 255));		// Mantemos costante os valores de RGB e mudamos somente o Alpha para 0.
				}

				// Aqui n�s mantemos a Nave em um Est�gio "inicial", inclusive excluindo quaisquer proj�til disparado
				ship.setPosition(window.getSize().x / 2, window.getSize().y / 2);
				ship.setRotation(0);
				sAngle = 0;
				sVelocity.x = 0; sVelocity.y = 0;
				listBullets.clear();				// Esse m�todo apaga todos os Proj�teis
				bVelocity.clear();
			}
			else
				ship.setColor(sf::Color(ship.getColor().r, ship.getColor().g, ship.getColor().b, 255));		// Mantemos costante os valores de RGB e mudamos somente o Alpha para 255.


			// ----------------------- Tratamento de Renderiza��es -----------------------
			// Limpa a Tela, preparando-a para Renderiza��es
			window.clear();

			// Desenha a Nave na (buffer de m�moria da) Janela
			window.draw(ship);

			// Desenha todos os Asteroids na (buffer de m�moria da) Janela
			for (int i = 0; i < listAsteroids.size(); i++)
				window.draw(listAsteroids[i]);

			// Desenha todos os Proj�teis na (buffer de m�moria da) Janela
			for (int i = 0; i < listBullets.size(); i++)
				window.draw(listBullets[i]);

			// EXTRA #02: HUD
			// Desenha a HUD na (buffer de mem�ria da) Janela
			for (int i = 0; i < sLife; i++)			// Perceba: n�s s� renderizamos a quantidade de Naves igual � de Life!
				window.draw(shipsAvailable[i]);

			window.draw(scoreText);

			// Exibe tudo que est� desenhado na (buffer de mem�ria da) Janela
			window.display();
		}
	}
	// ______________________________________________________________

	return 0;
}