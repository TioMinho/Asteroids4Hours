/*
		-----------------------------------------------------------------------------
						A S T E R O I D S     I N     4     H O U R S
		-----------------------------------------------------------------------------
															 		 by Minho Menezes

		O código abaixo corresponde ao resultado do Workshop "ASTEROIDS EM 4 HORAS: 
		INTRODUÇÃO À PROGRAMAÇÃO DE GAMES COM C++ E SFML".

		O intuito do Workshop foi apresentar, aos participantes, diversas técnicas e
		abordagens comuns à Programação de Games, apresentado-os a conceitos essenciais
		e familiarizando-os com o ambiente de conceber um jogo através da Programação.

		IMPORTANTE:
		Para que o código compile, e possa ser executado, é necessário criar um projeto
		com a biblioteca SFML configurada. Diversos tutoriais para instalação do SFML
		em diferentes IDE's podem ser encontrados pela internet; mais especificamente
		no próprio site da biblioteca: http://www.sfml-dev.org/

		Para mais informações, dúvidas ou curiosidades, me contate: 
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
	// --------------------------- Declarações ---------------------------
	// Janela
	// Estrutura do Construtor: ( [Dimensão], [Título], [Estilo] )
	sf::RenderWindow window(sf::VideoMode(800, 600), "Asteroids 4 Horas", sf::Style::Close);		// Janela do Jogo

	// Nave
	sf::Sprite ship;								// Objeto do tipo "sf::Sprite" que representa o Sprite
	sf::Texture shipTxt;							// Objeto do tipo "sf::Texture" que implementa a Textura do Sprite
	sf::Vector2f sVelocity(0, 0);					// Vetor Bi-dimensional do tipo Float (Vector-2-F) para manipular a Velocidade
	float sAngle = 0;								// Ângulo de Rotação da Nave
	sf::Clock bTrigger;								// Timer para controlar a frequência de disparos de Projéteis

	// Projétil
	std::vector<sf::RectangleShape> listBullets;	// Lista Encadeada de Projéteis do tipo "sf::RectangleShape"
	std::vector<sf::Vector2f> bVelocity;			// Lista Encadeada de Velocidade na forma de Vetor-2-Float (Espelhada a "listBullets")

	// Asteroids
	std::vector<sf::CircleShape> listAsteroids;		// Lista Encadeada de Asteroids do tipo "sf::CircleShape"
	std::vector<sf::Vector2f> aVelocity;			// Lista Encadeada de Velocidade na forma de Vetor-2-Float (Espelhada a "listAsteroids")
	
	// Moldes (ou Modelos)
	sf::CircleShape aMolde[3];						// Modelo para 3 Tipos de Asteroids
	sf::RectangleShape bMolde;						// Modelo uniforme para os Projéteis
	

	// ------------------------- Inicializações -------------------------
	// Inicialização da Janela
	window.setFramerateLimit(60);					// Limita o FPS em um teto de 60FPS, controlando a performance do Game.

	// Inicializações da Nave
	shipTxt.loadFromFile("res/ship.png");			// Carrega a imagem que está em "res/ship.png" para a textura "shipTxt".
	ship.setTexture(shipTxt);						// Seleciona "shipTxt" como Textura do sprite "ship"

	// Na linha a seguir, modificamos o Ponto de Origem do Sprite da Nave para sua posição central
	// Para isso, nós usamos o método ".getLocalBounds()" para recebermos o Retângulo que contorna o Sprite, e,
	// então, acessamos os atributos "width" e "height" desse Retângulo para a Dimensão do Sprite.
	ship.setOrigin(ship.getLocalBounds().width / 2, ship.getLocalBounds().height / 2);

	// Nessa linha modificamos a posição do Ponto de Origem do Sprite para o centro da Janela
	ship.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	// Inicialização do 3 Moldes dos Asteroids
	for (int i = 0; i < 3; i++)
	{
		aMolde[i].setFillColor(sf::Color::Transparent);			// Selecionamos a cor de preenchimento como "Transparente"
		aMolde[i].setPointCount(6);								// Indicamos que o Formato terá apenas 6 Arestas
		aMolde[i].setOutlineThickness(1 + i);					// Damos um valor de "1 + i" para a grossura da Linha de Contorno
		aMolde[i].setRadius(10 * (1 + i));						// Damos um valor de "10 * (1 + i)" para o Raio do Molde
		aMolde[i].setOrigin(5 * (1 + i), 5 * (1 + i));			// Modificamos o Ponto de Origem para o centro da Figura
	}

	// Inicialização do Molde dos Projéteis
	bMolde.setFillColor(sf::Color::White);						// Selecionamos a cor de preenchimento como "Branco"
	bMolde.setSize(sf::Vector2f(10, 4));						// Informamos as Dimensões do Projétil (10x4 pixels)
	bMolde.setOrigin(5, 2);										// Modificamos o Ponto de Origem para o centro da Figura

	// Inicialização do Seed para o Gerador Aleatório ( rand() )
	srand(time(NULL));

	// Game Loop ____________________________________________________
	while (window.isOpen())		// Lê-se: "Equanto a Janela está aberta".
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
			// Posicionamos e Rotacionamos o Molde do Projétil de acordo com a Nave
			bMolde.setPosition(ship.getPosition());
			bMolde.setRotation(ship.getRotation());

			// Geramos uma Velocidade baseada na rotação da Nave (sAngle) no momento do disparo.
			sf::Vector2f tempVel;
			tempVel.x = cos(sAngle * 3.1415 / 180) * 5;
			tempVel.y = sin(sAngle * 3.1415 / 180) * 5;

			// Inserimos o Projétil e sua Velocidade nas suas respectivas Listas
			listBullets.push_back(bMolde);
			bVelocity.push_back(tempVel);

			// Resetamos o Timer do Delay de 0.5 Segundos
			bTrigger.restart();
		}

		// Tecla "UP"
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			// Modificamos as componentes vetoriais da Velocidade na Nave de acordo com o seu ângulo de rotação
			sVelocity.x += cos(sAngle * 3.1415 / 180) * 0.02;
			sVelocity.y += sin(sAngle * 3.1415 / 180) * 0.02;
		}

		// Tecla "Left" e "Right":
		// Diminuimos ou Aumentamos, respectivamente, o valor do ângulo de Rotação.
		// Vale lembrar que o valor dos ângulos, em grau, crescem no sentido Horário!
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			sAngle -= 2;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			sAngle += 2;

		// ----------------------- Processamento -----------------------
		// Correção do Valor de Ângulo (Deve permanecer no intervalo 0~360)
		if (sAngle > 360)
			sAngle -= 360;
		else if (sAngle < 0)
			sAngle = +360;

		// Correção de Posicionamento da Nave
		// No nosso Algoritmo, nós verificamos se a posição da Nave ultrapassa algum dos
		// quatro limites da Janela, e então o "puxamos" para a outra extremidade oposta da
		// Janela usando o método ".move()".
		if (ship.getPosition().x < 0)
			ship.move(800, 0);
		else if (ship.getPosition().x > window.getSize().x)
			ship.move(-800, 0);

		if (ship.getPosition().y < 0)
			ship.move(0, 600);
		else if (ship.getPosition().y > window.getSize().y)
			ship.move(0, -600);

		// Execução da Rotação e Movimentação do Sprite da Nave
		ship.setRotation(sAngle);
		ship.move(sVelocity);

		// Execução do Movimentação de todos os Projéteis de "listBullets"
		for (int i = 0; i < listBullets.size(); i++)
			listBullets[i].move(bVelocity[i]);

		// Correção de Posicionamento dos Asteroids
		// Segue a mesma lógica da Correção de Posicionamento da Nave
		// Com a diferença de que a verificação deve ser feita para todos os
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

		// Execução do Movimentação de todos os Asteroids de "listAsteroids"
		for (int i = 0; i < listAsteroids.size(); i++)
			listAsteroids[i].move(aVelocity[i]);

		// Spawning Asteroids
		// Sempre que a lista de Asteroids estiver vazia, iremos criar novos 4
		// Asteroids, cada qual em um trecho aleatório das 4 extremidades da Janela.
		if (listAsteroids.empty())
		{
			for (int i = 0; i < 4; i++)
			{	
				// Consideramos os Asteroids pares (0 e 2) como os que aparecem nas extremidades Verticais
				if (i % 2 == 0)
				{
					// Geramos um valor aleatório para a posição no eixo X e posicionamos o molde de acordo.
					int posX = rand() % 800 + 1; // 0 - 800
					aMolde[2].setPosition(posX, i * 300);
				}
				// Consideramos os Asteroids pares (1 e 3) como os que aparecem nas extremidades Horizontais
				else
				{
					// Geramos um valor aleatório para a posição no eixo Y e posicionamos o molde de acordo.
					int posY = rand() % 600 + 1; // 0 - 800
					aMolde[2].setPosition((i - 1) * 400, posY);
				}

				// Criamos uma Velocidade e Rotação para o Asteroid.
				// Para isso, geramos um valor aleatório de 0~360 para o seu Ângulo de Rotação, e
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

		// ----------------------- Tratamento de Colisão -----------------------
		// Verificação e Tratamento de Colisão para a Colisão Asteroid-Projétil
		// Nós verificamores, para cada Asteroid, se o mesmo colide com qualquer Projétil
		// de "listBullets". Se sim, nós iremos apagar o referido Projétil da Lista,
		// gerar 2 Novos Asteroids (apenas se o referido Asteroid não for o menor) e
		// apagar o Asteroid colidido de "listAsteroids".
		for (int i = 0; i < listAsteroids.size(); i++)
		{
			for (int j = 0; j < listBullets.size(); j++)
			{
				// O "if" abaixo VERIFICA a Colisão
				if (listBullets[j].getGlobalBounds().intersects(listAsteroids[i].getGlobalBounds()))
				{
					// Apagamos o Projétil da posição "j" da lista "listBullets" (e apagamos sua velocidade
					// na lista espelhada "bVelocity").
					listBullets.erase(listBullets.begin() + j);
					bVelocity.erase(bVelocity.begin() + j);
					
					// Se o Asteroid da posição "i" não for o Menor (Raio igual a 10)...
					if (listAsteroids[i].getRadius() > 10)
					{
						// Criamos um índice para nos referirmos ao índice do Molde imediatamente menor
						// ao do Asteroid a ser destruído. Veja:
						// Asteroid Grande -> Molde[2] -> Raio = 30. (Raio / 10 - 2) == 1 -> Molde[1] < Molde[2]
						// Asteroid Médio -> Molde[1] -> Raio = 20. (Raio / 10 - 2) == 0 -> Molde[0] < Molde[1]
						int index = listAsteroids[i].getRadius() / 10 - 2;
						
						// Posicionamos o Molde dos Novos Asteroids para a posição do Asteroid Original
						aMolde[index].setPosition(listAsteroids[i].getPosition());

						// Geramos duas velocidades - uma para cada Asteroid que irá surgir -, onde cada nova
						// velocidade varia 45º do ângulo do vetor anterior. 
						sf::Vector2f tempVel[2];

						tempVel[0].x = cos((listAsteroids[i].getRotation() + 45) * 3.1415 / 180) * 1;
						tempVel[0].y = sin((listAsteroids[i].getRotation() + 45) * 3.1415 / 180) * 1;

						tempVel[1].x = cos((listAsteroids[i].getRotation() - 45) * 3.1415 / 180) * 1;
						tempVel[1].y = sin((listAsteroids[i].getRotation() - 45) * 3.1415 / 180) * 1;


						// Inserimos duas cópias iguais do Molde em "listAsteroids", e cada uma das suas 
						// respectivas velocidades na lista "aVelocity"
						listAsteroids.push_back(aMolde[index]); listAsteroids.push_back(aMolde[index]);
						aVelocity.push_back(tempVel[0]); aVelocity.push_back(tempVel[1]);
					}
					
					// Apagamos o Asteroid da posição "i" da lista "listAsteroid" (e apagamos sua velocidade
					// na lista espelhada "aVelocity").
					listAsteroids.erase(listAsteroids.begin() + i);
					aVelocity.erase(aVelocity.begin() + i);

					// Como a exclusão do Asteroid de "listAsteroid" fará com que os índices dos elementos
					// posteriores sejam diminuidos no valor de 1 (um), então devemos diminuir o índice do
					// "for" para garantirmos que nenhum Asteroid deixará de ser verificado.
					i--; break;
				}
			}
		}

		// ----------------------- Tratamento de Renderizações -----------------------
		// Limpa a Tela, preparando-a para Renderizações
		window.clear();										

		// Desenha a Nave na (buffer de mémoria da) Janela
		window.draw(ship);									

		// Desenha todos os Asteroids na (buffer de mémoria da) Janela
		for (int i = 0; i < listAsteroids.size(); i++)
			window.draw(listAsteroids[i]);

		// Desenha todos os Projéteis na (buffer de mémoria da) Janela
		for (int i = 0; i < listBullets.size(); i++)
			window.draw(listBullets[i]);

		// Exibe tudo que está desenhado na (buffer de memória da) Janela
		window.display();
	}
	// ______________________________________________________________

	return 0;
}







