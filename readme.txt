		-----------------------------------------------------------------------------
				 A S T E R O I D S     I N     4     H O U R S
		-----------------------------------------------------------------------------
							 		     by Minho Menezes

		O c�digo abaixo corresponde ao resultado do Workshop "ASTEROIDS EM 4 HORAS: 
		INTRODU��O � PROGRAMA��O DE GAMES COM C++ E SFML".

		O intuito do Workshop foi apresentar, aos participantes, diversas t�cnicas e
		abordagens comuns � Programa��o de Games, apresentado-os a conceitos essenciais
		e familiarizando-os com o ambiente de conceber um jogo atrav�s da Programa��o.

		!!!IMPORTANTE!!!:
		Para que o c�digo compile, e possa ser executado, � necess�rio criar um projeto
		com a biblioteca SFML configurada. Diversos tutoriais para instala��o do SFML
		em diferentes IDE's podem ser encontrados pela internet; mais especificamente
		no pr�prio site da biblioteca: http://www.sfml-dev.org/

		Instru��es GitHub:
		O c�digo e os arquivos de m�dia est�o dispon�veis para download.
		O c�digo que se encontra upado est� avan�ado ao conclu�do no Workshop.
		Fiz quest�o de adicionar os recursos de forma gradativa; assim, se voc�
		deseja visualizar como cada extra/otimiza��o foi feita, em rela��o ao
		c�digo original do Workshop, basta selecionar a op��o "Commits" aqui
		acima. Ela exibir� um "hist�rico" de modifica��es nos arquivos, e cada
		uma dessas modifica��es est� nomeada de acordo com o recurso adicionado
		ao jogo.
		O commit "Upload Code" representa o c�digo que n�s escrevemos durante
		o Workshop.

		CODE REPORT #01:
		Alguns programadores me contataram informando que as suas IDE's (basicamente, o
		Code::Blocks) n�o estavam reconhecendo a fun��o "std::to_string()", que converte
		dados para o tipo "std::string". Isso ocorre porque algumas dessas IDEs n�o v�m,
		por padr�o, configuradas para o C++1. Se voc� possui esse problema, � poss�vel 
		soluciona-lo mudando as configura��es do Code::Blocks ou da sua IDE para reconhecer 
		o C++11 ou, mais simples, substituir os trechos de c�digo que usam o "to_string()"
		pelos seguintes comandos:

			| std::stringstream convertStream;
			| convertStream << [vari�velNum�rica];
			| std::string converted = convertStream.str();

		Obs.: Lembrando que a solu��o acima s� funcionar� caso o programador inclua <sstream>
		como Biblioteca. 
	
		Para mais informa��es, d�vidas ou curiosidades, me contate: 
		E-mail: minhotmog@gmail.com
		
		E divirta-se! :)