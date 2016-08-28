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

	// Nave
	sf::Sprite ship;								// Objeto do tipo "sf::Sprite" que representa o Sprite
	sf::Texture shipTxt;							// Objeto do tipo "sf::Texture" que implementa a Textura do Sprite
	sf::Vector2f sVelocity(0, 0);					// Vetor Bi-dimensional do tipo Float (Vector-2-F) para manipular a Velocidade
	float sAngle = 0;								// �ngulo de Rota��o da Nave
	sf::Clock bTrigger;								// Timer para controlar a frequ�ncia de disparos de Proj�teis

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
		// N�s verificamores, para cada Asteroid, se o mesmo colide com qualquer Proj�til
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

		// Exibe tudo que est� desenhado na (buffer de mem�ria da) Janela
		window.display();
	}
	// ______________________________________________________________

	return 0;
}







