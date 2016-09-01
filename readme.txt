		-----------------------------------------------------------------------------
				 A S T E R O I D S     I N     4     H O U R S
		-----------------------------------------------------------------------------
							 		     by Minho Menezes

		O código abaixo corresponde ao resultado do Workshop "ASTEROIDS EM 4 HORAS: 
		INTRODUÇÃO À PROGRAMAÇÃO DE GAMES COM C++ E SFML".

		O intuito do Workshop foi apresentar, aos participantes, diversas técnicas e
		abordagens comuns à Programação de Games, apresentado-os a conceitos essenciais
		e familiarizando-os com o ambiente de conceber um jogo através da Programação.

		!!!IMPORTANTE!!!:
		Para que o código compile, e possa ser executado, é necessário criar um projeto
		com a biblioteca SFML configurada. Diversos tutoriais para instalação do SFML
		em diferentes IDE's podem ser encontrados pela internet; mais especificamente
		no próprio site da biblioteca: http://www.sfml-dev.org/

		Instruções GitHub:
		O código e os arquivos de mídia estão disponíveis para download.
		O código que se encontra upado está avançado ao concluído no Workshop.
		Fiz questão de adicionar os recursos de forma gradativa; assim, se você
		deseja visualizar como cada extra/otimização foi feita, em relação ao
		código original do Workshop, basta selecionar a opção "Commits" aqui
		acima. Ela exibirá um "histórico" de modificações nos arquivos, e cada
		uma dessas modificações está nomeada de acordo com o recurso adicionado
		ao jogo.
		O commit "Upload Code" representa o código que nós escrevemos durante
		o Workshop.

		CODE REPORT #01:
		Alguns programadores me contataram informando que as suas IDE's (basicamente, o
		Code::Blocks) não estavam reconhecendo a função "std::to_string()", que converte
		dados para o tipo "std::string". Isso ocorre porque algumas dessas IDEs não vêm,
		por padrão, configuradas para o C++1. Se você possui esse problema, é possível 
		soluciona-lo mudando as configurações do Code::Blocks ou da sua IDE para reconhecer 
		o C++11 ou, mais simples, substituir os trechos de código que usam o "to_string()"
		pelos seguintes comandos:

			| std::stringstream convertStream;
			| convertStream << [variávelNumérica];
			| std::string converted = convertStream.str();

		Obs.: Lembrando que a solução acima só funcionará caso o programador inclua <sstream>
		como Biblioteca. 
	
		Para mais informações, dúvidas ou curiosidades, me contate: 
		E-mail: minhotmog@gmail.com
		
		E divirta-se! :)