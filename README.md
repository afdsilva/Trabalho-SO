Trabalho-SO
===========

Trabalho 1 SO - UFPel 2013/2

Implementação Tabela Hash concorrente:

André Felipe da Silva
afdsilva@inf.ufpel.edu.br

Sistemas Operacionais - Prof. Maurício Lima Pilla

===========

	Compilação: make
	
===========
	
	Uso: ./teste <threads> <tamanho tabela> <nr blocos> <total operacoes>

===========
	
	Informações & metodologia: Minha ideia principal, foi criar uma classe (em teoria, para qualquer tipo de dados, o mais generica que pude) para utilização de uma tabela hash concorrente;
	a utilização da classe HashTable, constitui em utilizar o construtor informando o tamanho da tabela, o numero de blocos e a quantidade de threads que será executadas simultaneamente;
	ex:	HashTable<int> * hash = new HashTable<int>(50,5,4);
	Cria uma hashtable que guarda até 50 inteiros (relocação não foi utilizada, ou seja se adicionar além do limite da tabela, um erro é disparado, informando que a tabela está cheia),
	divido em 5 blocos, em que até 4 threads simultaneamente podem acessar paralelamente, para previnir a condição de corrida, o bloco só pode ser acessado por uma thread, utilizando um
	vetor de Mutexes que são selecionados de acordo com a posicao da chave a ser inserida, para tal o calculo feito é: posição / ( tamanho tabela / numero de blocos), isso garante que
	um mesmo bloco não será acessado por mais de uma thread concorrentemente, aumentando o numero de blocos permite uma maior subdivisão da tabela, os blocos devem ser multiplos do tamanho da
	tabela conforme especificado.
	Limite de Threads: quando o limite de threads em uma operação em lote (como no exemplo de teste), ultrapassa a quantidade de threads informada, a classe utiliza uma variavel de condição
	para limitar (wait) a quantidade de threads utilizada e é liberada (signal), em duas situações: a thread termina sua execução e informa a classe que está livre ou o metodo joinThreads()
	é chamado, esse comando usado também para liberar todas threads criadas e limpar a lista de threads, funciona como um pthreads_join global.
	
	A tabela hash implementa um tipo de arquivo hashObject, onde possui uma key e um value, value é o valor (ou chave) original utilizado com o metodo "add()", enquanto key é o resultado
	da aplicação da funcao hash, quando a key é diferente da posicao da tabela hash indica que houve colisao e uma nova posicao foi selecionada para aquele valor.
	
	Dois metodos foram implementados, além dos informados: search() e collision(), ambos metodos são utilizadas na hora da inserção, porem podem ser usadas publicamente. O primeiro, 
	busca se o valor já se encontra na tabela retornando 0 caso o nao seja encontrado, 1, objeto foi encontrado sem colisao, valor e key iguais, 2 o objeto foi encontrado com colisao, key diferente
	O metodo collision() apenas verifica se a posicao requisitada está vazia ou não;
	
	A biblioteca, já utiliza pthreads internamente para as funcoes add e set, em testes fechados, foi possível verificar a existencia do paralelismo na classe, pois uma sequencia adições
	em que existe colisões, a ordem de inserção não se mantinha consistente, comprovando que dependendo da escolha do escalonador, uma thread executa antes da outra, porém respeitando a
	consistência de dados da tabela, ressaltando que utilizar o print() ou printall() antes de joinThreads(), não garante que será impressa a tabela hash com seus valores finais, porém em
	todas operações é garantida a atomicidade, utilizando mutexes conforme o bloco.

	O programa de testes, realiza inserções e sets de numeros inteiros aleatórios (até 10.000), apesar de não testado, a classe foi desenvolvida para funcionar com qualquer tipo de dado 
	(com o uso de templates).
		
===========

	Classes Adicionais:
	hashObject: Tipo de dado abstrato que comporta um inteiro "key" e um tipo generico "value" (template);
	
	ThreadList: Utilizada para criar uma fila de threads, uma thread criada dentro de uma ThreadList sempre irá respeitar o limite de threads selecionado e irá "esperar" sua vez de execução
	utiliza mutex e variaveis de condição para esse fim;
	
	DataWrapper: Classe auxiliar que encapsula a tabela hash e o valor a ser adicionado ou setado, é enviado como parametro para a pthread_create (especie de gambiarra para manter os dados encapsulados)
	
	addWrapper e  setWrapper, são as funções estaticas que são chamadas pela pthread, utilizando a classe DataWrapper, mantem o encapsulamento dos dados da classe HashTable;
	
===========

	Bibliotecas adicionais utilizadas:
	Classes para Mutex/Variaveis de condição e Threads estilo Java: http://vichargrave.com/java-style-thread-class-in-c/
	* Apesar de só ter utilizado a classe Thread em testes iniciais, a classe Mutex e CondVar são usadas na biblioteca
