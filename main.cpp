#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void ADM_adicionar_produto(int ultimo_cod);

void listar_os_produtos(string vet_prod[70], float vet_preco[70], int vet_cod[70], int limit);

void importar_valores_dos_arquivos(string vetProd[70], float vetPreco[70], int vetCod[70], int vet_uni[70], float vet_margem[70]);

int limitador_vetor();

void processar_compra(float preco[70], int codigo, int unidades, int vet_unidades[70], int limit, float vetor_margem[70]);

void ADM_lucros_possiveis(float margem[70], int unidades[70], int limit);

bool verifica_estoque(int vet_unidades[70], int unidades, int codigo);

void ADM_adiciona_unidades_estoque(int vet_unidades[70], int limitador);

void ADM_verificar_lucro_atual();

void ADM_listar_os_produtos(string vet_prod[70], float vet_preco[70], int vet_cod[70], int limit, int vet_unidades[70]);

int main()
{
   //declaracao de variaveis main
   int opcao_inicial, codigo_produto, unidades, limitador, preco_final=0;
   int vetor_codigos[70] = {0}, vetor_unidades[70];
   float vetor_precos[70], margem_de_lucro[70];
   string vetor_produtos[70];
   bool resp_verifica_estoque;
   char resp;

   importar_valores_dos_arquivos(vetor_produtos, vetor_precos, vetor_codigos, vetor_unidades, margem_de_lucro);

   // defino de acordo com o numero de produtos q tenho um limite de tamanho dos vetores para facilitar na leitura.
   limitador = limitador_vetor();

   // para auxiliar quando for adicionar novos produtos, uso o ultimo codigo como base para adicionar os novos codigos de 1 em 1.
   int ultimo_valor_codigo = vetor_codigos[limitador-1];

   cout << "Bem vindo! Escolha uma opcao abaixo: \n\n";

   cout << "Usuario[1] ou Administrador[2]: ";
   cin >> opcao_inicial;
   cin.ignore();

   //garantir que escolha uma das opcoes
   while(opcao_inicial != 1 && opcao_inicial != 2)
   {
      cout << "Opcao invalida! Insira a opcao desejada: ";
      cin >> opcao_inicial;
   }

   //modo usuario
   if(opcao_inicial == 1)
   {
      cout << "\n";
      listar_os_produtos(vetor_produtos, vetor_precos, vetor_codigos, limitador);

      cout << "\n\nInforme o codigo do produto desejado: ";
      cin >> codigo_produto;

      cout << "\nInforme o numero de unidades desejadas: ";
      cin >> unidades;

      resp_verifica_estoque = verifica_estoque(vetor_unidades, unidades, codigo_produto);

      while(resp_verifica_estoque == false) // se der false pergunta para o cliente se quer
      {
         cout << "\nDeseja escolher outro produto ou diminuir o numero de unidades do produto desejado? [s/n]" << endl;

         cin >> resp;

         if(resp == 's' || resp == 'S')
         {
            cout << "\nInforme o codigo do produto desejado: ";
            cin >> codigo_produto;

            cout << "\nInforme o numero de unidades desejadas: ";
            cin >> unidades;

            verifica_estoque(vetor_unidades, unidades, codigo_produto);
         }
         else
            return 0;
      }

      processar_compra(vetor_precos, codigo_produto, unidades, vetor_unidades, limitador, margem_de_lucro);

   }

   int opcao; // para usar no menu ADM

   //modo ADM
   if(opcao_inicial == 2)
   {
      cout << "\n1) Verificar o maximo de lucro possivel com o estoque atual.";
      cout << "\n2) Adicionar um novo produto ao estoque.";
      cout << "\n3) Verificar lucro atual.";
      cout << "\n4) Adicionar mais unidades a um produto.";
      cout << "\n5) Listar Inventario dos produtos.";

      cout << "\n\nEscolha uma das opcoes acima: ";
      cin >> opcao;
      cin.ignore();

      switch(opcao)
      {
         case 1:
            ADM_lucros_possiveis(margem_de_lucro, vetor_unidades, limitador);
            break;

         case 2:
            ADM_adicionar_produto(ultimo_valor_codigo);
            break;

         case 3:
            ADM_verificar_lucro_atual();
            break;

         case 4:
            ADM_adiciona_unidades_estoque(vetor_unidades, limitador);
            break;

         case 5:
            ADM_listar_os_produtos(vetor_produtos, vetor_precos, vetor_codigos, limitador, vetor_unidades);
            break;

         default:
            cout << "\nOpcao inválida!" << endl;
            return 0;
      }
   }
   return 0;
}

void ADM_adiciona_unidades_estoque(int vet_unidades[70], int limitador){

   int codigo, unidades;

   cout << "\nDigite o codigo do produto que deseja acrescentar unidades: ";
   cin >> codigo;

   cout << "\nDigite o numero de unidades que deseja acrescentar: ";
   cin >> unidades;

   vet_unidades[codigo - 1] += unidades;

   ofstream guardar;
   guardar.open("unidades.txt");
   guardar << vet_unidades[0] << "\n";
   guardar.close();

   guardar.open("unidades.txt", ios::app);
   for(int i = 1; i < limitador; i++)
   {
      guardar << vet_unidades[i] << "\n";
   }
   guardar.close();
}

void ADM_verificar_lucro_atual(){

   float lucro;

   ifstream verificar;
   verificar.open("lucro.txt");
   verificar >> lucro;

   cout << "O lucro atual e = R$ " << lucro << endl;

}

bool verifica_estoque(int vet_unidades[70], int unidades, int codigo){

   char resp;

   if(vet_unidades[codigo - 1] < unidades)
   {
      cout << "\nEstoque insuficiente!" << endl;

      if(unidades > 1)
         cout << "O produto selecionado possui apenas " << vet_unidades[codigo - 1] << " disponiveis." << endl;

      return false;
   }

   return true;
}

void ADM_lucros_possiveis(float margem[70], int unidades[70], int limit){

   int i;
   float lucro_total=0;

   for(i = 0; i < limit; i++)
   {
      lucro_total += margem[i] * unidades[i];
   }

   cout << "\nCaso todos produtos sejam vendidos, teremos R$" << lucro_total << " de lucro." << endl;
}
void processar_compra(float preco[70], int codigo, int unidades, int vet_unidades[70], int limit, float vetor_margem[70]){

   float lucro_anterior;

   // pego o valor do lucro anterior do arquivo Lucro.txt.
   ifstream lucro;
   lucro.open("Lucro.txt");
   lucro >> lucro_anterior;

   char resp;
   float troco, dinheiro_recebido, preco_f, acumulador_lucro=lucro_anterior;
   int i=0;

   preco_f = preco[codigo - 1] * unidades; // calcula o preco final da compra

   acumulador_lucro += vetor_margem[codigo - 1] * unidades; // acumula o lucro da compra nessa variavel e depois guardo no arquivo txt

   vet_unidades[codigo - 1] -= unidades; // retira as unidades compradas do estoque

   cout << "Deseja adicionar mais algum produto? [s/n]" << endl;
   cin >> resp;

   while(resp == 's' || resp == 'S') // caso queira adicionar mais produtos
   {
       cout << "\nInforme  o codigo do produto desejado: ";
       cin >> codigo;

       cout << "\nInforme o numero de unidades que deseja comprar: ";
       cin >> unidades;

       preco_f += preco[codigo - 1] * unidades;

       acumulador_lucro += vetor_margem[codigo - 1] * unidades;

       vet_unidades[codigo - 1] -= unidades;

       cout << "\nDeseja adicionar mais algum produto? [s/n]" << endl;
       cin >> resp;
   }

   cout << "\nValor total = R$" << preco_f;

   cout << "\nInsira o valor para realizar o pagamento: ";
   cin >> dinheiro_recebido;

   troco = dinheiro_recebido - preco_f; // calcula troco

   while(troco < 0) // caso saldo insuficiente
   {
      cout << "\nValor insuficiente!";
      cout << "\nDigite o valor novamente: ";
      cin >> dinheiro_recebido;

      troco = dinheiro_recebido - preco_f;
   }

   cout << "\nSeu troco e = R$" << troco << endl;

   cout << "\n\nObrigado!";

   // trocar o conteudo do arquivo com as unidades dos produtos antigas pelas novas apos a venda
   ofstream unidades_arquivo;
   unidades_arquivo.open("unidades.txt");
   unidades_arquivo << vet_unidades[0] << "\n"; // primeiro uso sem (ios::app) para sobreescrever tudo q tem no arquivo
   unidades_arquivo.close();

   unidades_arquivo.open("unidades.txt", ios::app); // abro novamente mas com (ios::app) para adicionar os demais valores do vetor_unidades

   for(i = 1; i < limit; i++)
   {
      unidades_arquivo << vet_unidades[i] << "\n";
   }
   unidades_arquivo.close();

   //Guardo o lucro acumulado trocando o valor anterior pelo atual.
   ofstream guardar_lucro;
   guardar_lucro.open("Lucro.txt");
   guardar_lucro << acumulador_lucro;
   guardar_lucro.close();
}

int limitador_vetor(){

   int indice=0, vetCod[70], limit=0;

   ifstream codigos;
   codigos.open("Codigo.txt");
   while(codigos >> vetCod[indice])
   {
      indice++;
      limit++;
   }
   codigos.close();

   return limit;
}

void importar_valores_dos_arquivos(string vetProd[70], float vetPreco[70], int vetCod[70], int vet_uni[70], float vet_margem[70]){

   int indice = 0;
   string linha;

   // uso while para rodar cada arquivo e salvar os valores em seus respectivos vetores.
   //produto
   ifstream produto;
   produto.open("produtos.txt");
   while(getline(produto, linha))
   {
      vetProd[indice] = linha;
      indice++;
   }
   produto.close();

   //codigos
   indice=0; // reseta o indice para usar de contador
   ifstream codigos;
   codigos.open("Codigo.txt");
   while(codigos >> vetCod[indice])
      indice++;
   codigos.close();

   //precos
   indice=0;
   ifstream precos;
   precos.open("preco_venda.txt");
   while(precos >> vetPreco[indice])
      indice++;
   precos.close();

   //unidades
   indice=0;
   ifstream unidades;
   unidades.open("unidades.txt");
   while(unidades >> vet_uni[indice])
      indice++;
   unidades.close();

   //margem
   indice=0;
   ifstream margem;
   margem.open("Margem_de_lucro.txt");
   while(margem >> vet_margem[indice])
      indice++;
   margem.close();

}

void ADM_adicionar_produto(int ultimo_cod){

   int vetor_teste[1];
   bool tem_algo = false;
   char resposta = 's';
   string nome_produto;
   int unidades_do_produto, codigo_do_produto;
   double preco_venda, lucro;

   ifstream teste_cod;  // se for o primeiro produto, ira definir o primeiro valor de codigo de produto como 1
   teste_cod.open("Codigo.txt");
   while(teste_cod >> vetor_teste[0])
      tem_algo = true;
   teste_cod.close();
   if(!tem_algo)
      ultimo_cod = 0;

   ofstream produto;
   produto.open("Produtos.txt", ios::app);

   // abro os arquivos para inserir os valores
   ofstream codigo;
   codigo.open("Codigo.txt", ios::app);

   ofstream unidades;
   unidades.open("Unidades.txt", ios::app);

   ofstream preco_v;
   preco_v.open("Preco_venda.txt", ios::app);

   ofstream margem;
   margem.open("Margem_de_lucro.txt", ios::app);

   while(resposta == 's' || resposta == 'S')
   {
      ultimo_cod++;

      // usando a biblioteca fstream armazeno o inventario dos produtos da maquina nos arquivos .txt correspondentes ao codigo, preco, nome, lucro e qtd de unidades.
      cout << "\nInforme o nome do produto: "; // produto
      getline(cin, nome_produto);
      produto << nome_produto << "\n";

      codigo << ultimo_cod << "\n"; // codigo

      cout << "\nInforme o numero de unidades armazenadas do produto que sera adicionado: "; // unidades
      cin >> unidades_do_produto;
      unidades << unidades_do_produto << "\n";

      cout << "\nInforme o preco que o produto sera vendido: "; // preco de venda por unidades
      cin >> preco_venda;
      preco_v << preco_venda << "\n";

      cout << "\nInforme a margem de lucro por unidade vendida do produto: ";
      cin >> lucro;
      margem << lucro << "\n";

      cout << "\nDeseja adicionar mais algum produto? [s/n]\n";
      cin >> resposta;
      cin.ignore();
   }
   //fechando os arquivos de texto com os valores armazenados
   codigo.close();
   unidades.close();
   preco_v.close();
   margem.close();
}

void listar_os_produtos(string vet_prod[70], float vet_preco[70], int vet_cod[70], int limit){

    int espaco;
    string palavra;

    cout << "Codigo    ";
    cout << "Produto                 ";
    cout << "Preco" << endl;

    for(int i = 0; i < limit; i++)
    {
       palavra = vet_prod[i];

       cout << vet_cod[i] << "         " << vet_prod[i];

       espaco = 25 - palavra.length();

       for(int j = 1; j < espaco; j++)
       {
          cout << " ";
       }

       cout << "R$" << vet_preco[i] << endl;
    }

}

void ADM_listar_os_produtos(string vet_prod[70], float vet_preco[70], int vet_cod[70], int limit, int vet_unidades[70])
{

   int espaco;
   string palavra;

   cout << "\nCodigo    ";
   cout << "Unidades    ";
   cout << "Produto                 ";
   cout << "Preco" << endl;

   for(int i = 0; i < limit; i++)
   {
      palavra = vet_prod[i];

      cout << vet_cod[i] << "           " << vet_unidades[i] << "         " << vet_prod[i];

      espaco = 25 - palavra.length();

      for(int j = 1; j < espaco; j++)
      {
         cout << " ";
      }

      cout << "R$" << vet_preco[i] << endl;
   }
}
