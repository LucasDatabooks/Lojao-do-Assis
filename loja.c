#include <stdio.h>      // Biblioteca padrão pra entrada, saída e manipulação de arquivos
#include <stdlib.h>     // Biblioteca padrão pra funções utilitárias
#include <string.h>     // Biblioteca pra manipulação de strings (strcpy, strcspn, etc.)
#include <strings.h>    // Biblioteca pra compração case-insensitive (strcasecmp)
#include <ctype.h>      // Biblioteca pra manipulação de caracteres (tolower)

#define MAX_PRODUTOS 100 // Define a capacidade máxima do vetor de produtos

// Estrutura que representa um produto no cadastro da loja
typedef struct {
    int id;                 // identificador único gerado automaticamente
    char nome[50];          // Nome do produto
    char categoria[50];     // Categoria do produto (adicionado conforme enunciado)
    char fornecedor[50];    // Nome do fornecedor
    int quantidade;         // Quantidade disponível em estoque
    float preco;            // Preço unitário do produto
} Produto;

Produto lista[MAX_PRODUTOS]; // Vetor principal de estruturas armazenado em memória
int total_produtos = 0;      // Contador de produtos atualmente cadastrados
int proximo_id = 1;          // Variável global que controla o id incremental

// Limpa caracteres residuais do buffer de entrada (stdin)
void limpar_buffer() {
    int c; // Variável pra armazenar cada caractere lido
    while ((c = getchar()) != '\n' && c != EOF); // Descarta até encontrar quebra de linha ou fim de arquivo
}

// Compra duas strings ignorando diferenças entre maiúsculas e minúsculas (case-insensitive)
int comprar_ignorando_caso(const char *s1, const char *s2) {
    while (*s1 && *s2) { // Percorre ambas as strings simultaneamente via ponteiros
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) { // Compra caracteres em minúsculo
            return 1; // Retorna 1 se forem diferentes
        }
        s1++; // Avança o ponteiro da primeira string
        s2++; // Avança o ponteiro da segunda string
    }
    return tolower((unsigned char)*s1) != tolower((unsigned char)*s2); // Verifica se ambas terminaram juntas (0 = iguais)
}

// Verifica se uma substring existe dentro de outra string ignorando maiúsculas/minúsculas
int contem_ignorando_caso(const char *texto, const char *busca) {
    if (*busca == '\0') return 1; // String de busca vazia
    for (; *texto != '\0'; texto++) { // Avança caractere por caractere no texto principal
        const char *t = texto; // Ponteiro auxiliar pra testar a partir da posição atual
        const char *b = busca; // Ponteiro auxiliar pra percorrer o termo buscado
        while (*t != '\0' && *b != '\0' && tolower((unsigned char)*t) == tolower((unsigned char)*b)) {
            t++; // Avança no texto
            b++; // Avança na busca
        }
        if (*b == '\0') return 1; // Se chegou ao fim da busca, encontrou correspondência
    }
    return 0; // Não encontrou correspondência
}

// Verifica se um determinado id já está em uso no vetor de produtos
int id_existe(Produto *vet, int total, int id) {
    for (int i = 0; i < total; i++) { // Percorre todos os produtos cadastrados
        if ((vet + i)->id == id) { // Acessa o campo id via aritmética de ponteiros
            return 1; // Retorna 1 indicando que o id já existe
        }
    }
    return 0; // Retorna 0 se o id estiver livre
}

// Carrega os registros salvos no arquivo texto pra o vetor em memória ao iniciar
void carregar_dados(Produto *vet, int *total, int *prox_id) {
    FILE *arquivo = fopen("produtos.txt", "r"); // Abre o arquivo de dados em modo leitura
    if (arquivo == NULL) { // Verifica se o arquivo ainda não existe
        return; // Encerra a função mantendo o cadastro vazio e id inicial 1
    }

    *total = 0; // Reinicia o contador de produtos em memória
    Produto p;  // Estrutura temporária pra leitura de cada linha

    // Lê os campos formatados de cada linha do arquivo texto
    while (fscanf(arquivo, "id: %d | Nome: %49[^|] | Categoria: %49[^|] | Fornecedor: %49[^|] | Qtd: %d | Preço: %f\n",
                  &p.id, p.nome, p.categoria, p.fornecedor, &p.quantidade, &p.preco) == 6) {
        
        // Remove o espaço em branco residual antes do seprador '|' em cada string lida
        size_t len_nome = strlen(p.nome);
        if (len_nome > 0 && p.nome[len_nome - 1] == ' ') p.nome[len_nome - 1] = '\0';

        size_t len_cat = strlen(p.categoria);
        if (len_cat > 0 && p.categoria[len_cat - 1] == ' ') p.categoria[len_cat - 1] = '\0';

        size_t len_forn = strlen(p.fornecedor);
        if (len_forn > 0 && p.fornecedor[len_forn - 1] == ' ') p.fornecedor[len_forn - 1] = '\0';

        // Insere no vetor apenas se houver espaço e se o id não for duplicado
        if (*total < MAX_PRODUTOS && !id_existe(vet, *total, p.id)) {
            vet[*total] = p; // Armazena o produto na posição atual
            (*total)++;      // Incrementa o total de produtos carregados
            if (p.id >= *prox_id) { // Atualiza o próximo id disponível pra evitar colisões
                *prox_id = p.id + 1;
            }
        }
    }

    fclose(arquivo); // Fecha o arquivo após concluir a leitura
}

// Cria (ou reinicia) o arquivo principal de produtos (adaptado de gerarlistaprodutos do Lucas)
void criar_arquivo_dados(Produto *vet, int *total, int *prox_id) {
    char confirmacao; // Variável pra confirmar a ação com o usuário
    printf("Deseja criar/zerar o arquivo produtos.txt? Isso apagara registros existentes (s/n): ");
    scanf(" %c", &confirmacao); // Lê a resposta ignorando espaços em branco
    limpar_buffer(); // Limpa o buffer de entrada

    if (tolower((unsigned char)confirmacao) != 's') { // Cancela se o usuário não digitar 's'
        printf("Operacao cancelada.\n");
        return;
    }

    FILE *fp = fopen("produtos.txt", "w"); // Abre/cria o arquivo em modo de escrita (sobrescreve)
    FILE *fp_temp = fopen("temp_produtos.txt", "w"); // Testa também a criação do arquivo temporário
    if (fp == NULL || fp_temp == NULL) { // Verifica falha na criação dos arquivos
        printf("Erro ao criar arquivos.\n");
        if (fp) fclose(fp);
        if (fp_temp) fclose(fp_temp);
        return;
    }

    fclose(fp); // Fecha o arquivo principal
    fclose(fp_temp); // Fecha o arquivo temporário
    remove("temp_produtos.txt"); // Remove o temporário vazio

    *total = 0; // Zera a quantidade de produtos em memória
    *prox_id = 1; // Reinicia o contador de id incremental
    (void)vet; // Mantém compatibilidade com a assinatura da função
    printf("Arquivo produtos.txt criado/reiniciado com sucesso!\n");
}

// Exibe os produtos de um vetor em formato de tabela alinhada na tela
void imprimir_tabela(const char *titulo, Produto *vet, int total) {
    if (total == 0) { // Verifica se há registros pra exibir
        printf("\nNenhum produto encontrado pra exibir.\n");
        return;
    }

    printf("\n=== %s ===\n", titulo); // Imprime o título da visualização atual
    printf("+-------+---------------------------+--------------------+----------------------+--------+------------+\n");
    printf("| %-5s | %-25s | %-18s | %-20s | %-6s | %-10s |\n", "id", "NOME", "CATEGORIA", "FORNECEDOR", "QTD", "PREÇO");
    printf("+-------+---------------------------+--------------------+----------------------+--------+------------+\n");
    
    for (int i = 0; i < total; i++) { // Percorre o vetor usando ponteiro pra imprimir cada linha
        Produto *p = &vet[i]; // Ponteiro apontando pra o produto atual
        printf("| %-5d | %-25s | %-18s | %-20s | %-6d | R$ %7.2f |\n",
               p->id, p->nome, p->categoria, p->fornecedor, p->quantidade, p->preco);
    }
    
    printf("+-------+---------------------------+--------------------+----------------------+--------+------------+\n");
}

// Insere um novo produto no vetor e grava no final do arquivo texto
void inserir_produto(Produto *vet, int *total, int *prox_id) {
    if (*total >= MAX_PRODUTOS) { // Impede estouro da capacidade do vetor
        printf("Limite maximo de produtos atingido.\n");
        return;
    }

    char nome[50], categoria[50], fornecedor[50]; // Buffers temporários pra leitura
    int quantidade; // Armazena a quantidade digitada
    float preco;    // Armazena o preço digitado

    // Leitura e validação do NOME
    printf("Digite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin); // Lê o nome com espaços
    nome[strcspn(nome, "\n")] = '\0'; // Remove o '\n' do final da string

    if (nome[0] == '\0') { // Valida se o campo não ficou vazio
        printf("O nome do produto nao pode ser vazio.\n");
        return;
    }

    // Verifica duplicidade de nome ignorando maiúsculas/minúsculas
    for (int i = 0; i < *total; i++) {
        if (comprar_ignorando_caso(vet[i].nome, nome) == 0) {
            printf("Erro: Produto com este nome ja esta cadastrado!\n");
            return;
        }
    }

    // Leitura e validação da CATEGORIA
    printf("Digite a categoria do produto: ");
    fgets(categoria, sizeof(categoria), stdin); // Lê a categoria
    categoria[strcspn(categoria, "\n")] = '\0'; // Remove o '\n'

    if (categoria[0] == '\0') { // Valida se a categoria não está vazia
        printf("A categoria nao pode ser vazia.\n");
        return;
    }

    // Leitura e validação do FORNECEDOR
    printf("Digite o fornecedor do produto: ");
    fgets(fornecedor, sizeof(fornecedor), stdin); // Lê o fornecedor
    fornecedor[strcspn(fornecedor, "\n")] = '\0'; // Remove o '\n'

    if (fornecedor[0] == '\0') { // Valida se o fornecedor não está vazio
        printf("O fornecedor nao pode ser vazio.\n");
        return;
    }

    // Leitura e validação da QUANTidADE
    printf("Digite a quantidade do produto: ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 0) { // Garante número inteiro positivo
        printf("Quantidade invalida. Deve ser maior que zero.\n");
        limpar_buffer();
        return;
    }

    // Leitura e validação do PREÇO
    printf("Digite o preco do produto: ");
    if (scanf("%f", &preco) != 1 || preco <= 0) { // Garante valor decimal positivo
        printf("Preco invalido. Deve ser maior que zero.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer(); // Limpa o '\n' deixado pelo scanf

    // Garante que o id incremental gerado não seja duplicado
    while (id_existe(vet, *total, *prox_id)) {
        (*prox_id)++; // Incrementa até encontrar um id livre
    }

    // Preenche a estrutura na próxima posição livre do vetor usando ponteiro
    Produto *novo = &vet[*total];
    novo->id = (*prox_id)++;
    strcpy(novo->nome, nome);
    strcpy(novo->categoria, categoria);
    strcpy(novo->fornecedor, fornecedor);
    novo->quantidade = quantidade;
    novo->preco = preco;
    (*total)++; // Atualiza o contador total de produtos

    // Abre o arquivo em modo append ("a") pra adicionar o novo registro sem apagar os anteriores
    FILE *arquivo = fopen("produtos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo produtos.txt!\n");
        return;
    }

    // Grava o registro formatado no arquivo
    fprintf(arquivo, "id: %d | Nome: %s | Categoria: %s | Fornecedor: %s | Qtd: %d | Preço: %.2f\n",
            novo->id, novo->nome, novo->categoria, novo->fornecedor, novo->quantidade, novo->preco);
    fclose(arquivo); // Fecha o arquivo

    printf("Produto cadastrado e gravado com sucesso! (id: %d)\n", novo->id);
}

// Altera um produto existente utilizando arquivo temporário pra evitar corrupção (lógica do Lucas)
void alterar_produto(Produto *vet, int total) {
    if (total == 0) { // Verifica se existem produtos cadastrados
        printf("Nenhum produto cadastrado pra alterar.\n");
        return;
    }

    int id_alvo; // Armazena o id do produto que o usuário deseja editar
    printf("Digite o id do produto que deseja alterar: ");
    if (scanf("%d", &id_alvo) != 1) { // Valida a leitura numérica
        printf("id invalido!\n");
        limpar_buffer();
        return;
    }
    limpar_buffer(); // Limpa o buffer

    // Localiza o índice do produto no vetor em memória
    int indice = -1;
    for (int i = 0; i < total; i++) {
        if (vet[i].id == id_alvo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) { // Se não encontrou o id, encerra
        printf("Produto com id %d nao encontrado.\n", id_alvo);
        return;
    }

    Produto *p = &vet[indice]; // Ponteiro pra o produto que será editado
    char novo_nome[50], nova_cat[50], novo_forn[50];
    int nova_qtd;
    float novo_preco;

    printf("Editando produto id %d (%s):\n", p->id, p->nome);

    // Lê e valida o novo nome
    printf("Novo nome: ");
    fgets(novo_nome, sizeof(novo_nome), stdin);
    novo_nome[strcspn(novo_nome, "\n")] = '\0';
    if (novo_nome[0] == '\0') {
        printf("Nome nao pode ser vazio.\n");
        return;
    }

    // Verifica se o novo nome já pertence a outro produto diferente deste
    for (int i = 0; i < total; i++) {
        if (i != indice && comprar_ignorando_caso(vet[i].nome, novo_nome) == 0) {
            printf("Erro: Ja existe outro produto com este nome.\n");
            return;
        }
    }

    // Lê e valida a nova categoria
    printf("Nova categoria: ");
    fgets(nova_cat, sizeof(nova_cat), stdin);
    nova_cat[strcspn(nova_cat, "\n")] = '\0';
    if (nova_cat[0] == '\0') {
        printf("Categoria nao pode ser vazia.\n");
        return;
    }

    // Lê e valida o novo fornecedor
    printf("Novo fornecedor: ");
    fgets(novo_forn, sizeof(novo_forn), stdin);
    novo_forn[strcspn(novo_forn, "\n")] = '\0';
    if (novo_forn[0] == '\0') {
        printf("Fornecedor nao pode ser vazio.\n");
        return;
    }

    // Lê e valida a nova quantidade
    printf("Nova quantidade: ");
    if (scanf("%d", &nova_qtd) != 1 || nova_qtd <= 0) {
        printf("Quantidade invalida.\n");
        limpar_buffer();
        return;
    }

    // Lê e valida o novo preço
    printf("Novo preco: ");
    if (scanf("%f", &novo_preco) != 1 || novo_preco <= 0) {
        printf("Preco invalido.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    // Atualiza os dados na estrutura em memória
    strcpy(p->nome, novo_nome);
    strcpy(p->categoria, nova_cat);
    strcpy(p->fornecedor, novo_forn);
    p->quantidade = nova_qtd;
    p->preco = novo_preco;

    // Abre o arquivo original pra leitura e um arquivo temporário pra gravação segura
    FILE *fp = fopen("produtos.txt", "r");
    FILE *templist = fopen("temp_produtos.txt", "w");
    if (templist == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        if (fp) fclose(fp);
        return;
    }

    // Regrava todos os registros atualizados no arquivo temporário
    for (int i = 0; i < total; i++) {
        fprintf(templist, "id: %d | Nome: %s | Categoria: %s | Fornecedor: %s | Qtd: %d | Preço: %.2f\n",
                vet[i].id, vet[i].nome, vet[i].categoria, vet[i].fornecedor, vet[i].quantidade, vet[i].preco);
    }

    if (fp != NULL) fclose(fp); // Fecha o arquivo original antes de remover
    fclose(templist); // Fecha o arquivo temporário antes de renomear

    remove("produtos.txt"); // Remove o arquivo antigo
    rename("temp_produtos.txt", "produtos.txt"); // Substitui pelo arquivo temporário atualizado

    printf("Produto id %d alterado com sucesso!\n", id_alvo);
}

// Exclui um produto pelo id utilizando arquivo temporário (técnica de segurança de dados)
void excluir_produto(Produto *vet, int *total) {
    if (*total == 0) { // Verifica se há registros pra excluir
        printf("Nenhum produto cadastrado pra excluir.\n");
        return;
    }

    int id_alvo; // id do produto a ser removido
    printf("Digite o id do produto que deseja excluir: ");
    if (scanf("%d", &id_alvo) != 1) {
        printf("id invalido!\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    // Procura o produto no vetor
    int indice = -1;
    for (int i = 0; i < *total; i++) {
        if (vet[i].id == id_alvo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) { // Caso o id não exista
        printf("Produto com id %d nao encontrado.\n", id_alvo);
        return;
    }

    // Desloca os elementos seguintes uma posição pra trás pra remover o item do vetor
    for (int i = indice; i < (*total) - 1; i++) {
        vet[i] = vet[i + 1];
    }
    (*total)--; // Decrementa o total de produtos ativos

    // Abre o original e o arquivo temporário
    FILE *fp = fopen("produtos.txt", "r");
    FILE *templist = fopen("temp_produtos.txt", "w");
    if (templist == NULL) {
        printf("Erro ao criar arquivo temporario.\n");
        if (fp) fclose(fp);
        return;
    }

    // Escreve no arquivo temporário apenas os produtos que permaneceram no vetor
    for (int i = 0; i < *total; i++) {
        fprintf(templist, "id: %d | Nome: %s | Categoria: %s | Fornecedor: %s | Qtd: %d | Preço: %.2f\n",
                vet[i].id, vet[i].nome, vet[i].categoria, vet[i].fornecedor, vet[i].quantidade, vet[i].preco);
    }

    if (fp != NULL) fclose(fp); // Fecha o arquivo original
    fclose(templist); // Fecha o temporário

    remove("produtos.txt"); // Apaga o arquivo antigo
    rename("temp_produtos.txt", "produtos.txt"); // Renomeia o temporário pra o nome oficial

    printf("Produto id %d excluido com sucesso!\n", id_alvo);
}

// Busca produtos por nome (case-insensitive) e exibe o resultado em tabela
void buscar_por_nome(Produto *vet, int total) {
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    char termo[50]; // Termo de busca digitado pelo usuário
    printf("Digite o nome pra buscar: ");
    fgets(termo, sizeof(termo), stdin);
    termo[strcspn(termo, "\n")] = '\0';

    Produto encontrados[MAX_PRODUTOS]; // Vetor auxiliar pra armazenar os resultados
    int qtd_encontrados = 0; // Contador de correspondências

    for (int i = 0; i < total; i++) { // Percorre toda a lista de produtos
        if (contem_ignorando_caso(vet[i].nome, termo)) { // Compração case-insensitive
            encontrados[qtd_encontrados++] = vet[i]; // Copia o produto encontrado
        }
    }

    if (qtd_encontrados == 0) {
        printf("Nenhum produto encontrado com o nome \"%s\".\n", termo);
    } else {
        imprimir_tabela("RESULTADO DA BUSCA POR NOME", encontrados, qtd_encontrados);
    }
}

// Busca produtos por categoria (case-insensitive) e exibe o resultado em tabela
void buscar_por_categoria(Produto *vet, int total) {
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    char termo[50]; // Categoria digitada pelo usuário
    printf("Digite a categoria pra buscar: ");
    fgets(termo, sizeof(termo), stdin);
    termo[strcspn(termo, "\n")] = '\0';

    Produto encontrados[MAX_PRODUTOS]; // Vetor auxiliar pra armazenar os resultados
    int qtd_encontrados = 0; // Contador de correspondências

    for (int i = 0; i < total; i++) { // Percorre toda a lista de produtos
        if (contem_ignorando_caso(vet[i].categoria, termo)) { // Compração case-insensitive
            encontrados[qtd_encontrados++] = vet[i]; // Copia o produto encontrado
        }
    }

    if (qtd_encontrados == 0) {
        printf("Nenhum produto encontrado na categoria \"%s\".\n", termo);
    } else {
        imprimir_tabela("RESULTADO DA BUSCA POR CATEGORIA", encontrados, qtd_encontrados);
    }
}

// Ordena e exibe os produtos sem alterar o arquivo original (Bubble Sort)
// modo: 1 = Nome (A-Z) | 2 = Nome (Z-A) | 3 = Preço (Menor -> Maior) | 4 = Preço (Maior -> Menor)
void ordenar_e_exibir(Produto *vet, int total, int modo) {
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    // Cria uma cópia temporária do vetor pra não alterar a ordem original nem o arquivo
    Produto copia[MAX_PRODUTOS];
    for (int i = 0; i < total; i++) {
        copia[i] = vet[i];
    }

    Produto temp; // Variável auxiliar pra a troca de posições no Bubble Sort

    // Algoritmo Bubble Sort com ponteiros
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            Produto *a = &copia[j];     // Ponteiro pra o elemento atual
            Produto *b = &copia[j + 1]; // Ponteiro pra o próximo elemento
            int trocar = 0;             // Flag que indica se deve trocar os elementos

            if (modo == 1 && strcasecmp(a->nome, b->nome) > 0) {
                trocar = 1; // Nome A-Z (case-insensitive)
            } else if (modo == 2 && strcasecmp(a->nome, b->nome) < 0) {
                trocar = 1; // Nome Z-A (case-insensitive)
            } else if (modo == 3 && a->preco > b->preco) {
                trocar = 1; // Preço Menor -> Maior
            } else if (modo == 4 && a->preco < b->preco) {
                trocar = 1; // Preço Maior -> Menor
            }

            if (trocar) { // Realiza a troca das estruturas
                temp = *a;
                *a = *b;
                *b = temp;
            }
        }
    }

    // Exibe a tabela correspondente ao critério escolhido
    if (modo == 1) imprimir_tabela("PRODUTOS ORDENADOS POR NOME (A - Z)", copia, total);
    else if (modo == 2) imprimir_tabela("PRODUTOS ORDENADOS POR NOME (Z - A)", copia, total);
    else if (modo == 3) imprimir_tabela("PRODUTOS ORDENADOS POR PRECO (MENOR -> MAIOR)", copia, total);
    else if (modo == 4) imprimir_tabela("PRODUTOS ORDENADOS POR PRECO (MAIOR -> MENOR)", copia, total);
}

// Exporta todos os dados do vetor pra um arquivo CSV seprado por vírgulas
void exportar_csv(Produto *vet, int total) {
    if (total == 0) {
        printf("Nenhum produto cadastrado pra exportar.\n");
        return;
    }

    FILE *csv = fopen("produtos.csv", "w"); // Abre/cria o arquivo CSV pra escrita
    if (csv == NULL) {
        printf("Erro ao criar o arquivo produtos.csv!\n");
        return;
    }

    // Escreve o cabeçalho das colunas seprado por vírgula
    fprintf(csv, "id,Nome,Categoria,Fornecedor,Quantidade,Preco\n");

    // Escreve cada produto em uma linha do CSV
    for (int i = 0; i < total; i++) {
        Produto *p = &vet[i];
        fprintf(csv, "%d,\"%s\",\"%s\",\"%s\",%d,%.2f\n", p->id, p->nome, p->categoria, p->fornecedor, p->quantidade, p->preco);
    }

    fclose(csv); // Fecha o arquivo CSV
    printf("Dados exportados com sucesso pra 'produtos.csv'!\n");
}

// Função principal com o menu interativo do sistema
int main() {
    carregar_dados(lista, &total_produtos, &proximo_id); // Carrega registros existentes ao iniciar
    int opcao; // Armazena a escolha do usuário no menu

    do {
        printf("\n SISTEMA DE CADASTRO DE PRODUTOS \n");
        printf("1. Criar/Reiniciar arquivo de dados\n");
        printf("2. Inserir produto\n");
        printf("3. Listar todos os produtos\n");
        printf("4. Alterar produto\n");
        printf("5. Excluir produto\n");
        printf("6. Buscar produto por nome\n");
        printf("7. Buscar produto por categoria\n");
        printf("8. Ordenar por nome (A - Z)\n");
        printf("9. Ordenar por nome (Z - A)\n");
        printf("10. Ordenar por preco (Menor -> Maior)\n");
        printf("11. Ordenar por preco (Maior -> Menor)\n");
        printf("12. Exportar dados pra CSV\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) { // Valida entrada numérica do menu
            printf("Opcao invalida!\n");
            limpar_buffer();
            opcao = -1;
            continue;
        }
        limpar_buffer(); // Limpa o '\n' restante

        switch (opcao) {
            case 1: criar_arquivo_dados(lista, &total_produtos, &proximo_id);
                break;
            case 2: inserir_produto(lista, &total_produtos, &proximo_id);
                break;
            case 3: imprimir_tabela("LISTA GERAL DE PRODUTOS", lista, total_produtos);
                break;
            case 4: alterar_produto(lista, total_produtos);
                break;
            case 5: excluir_produto(lista, &total_produtos);
                break;
            case 6: buscar_por_nome(lista, total_produtos);
                break;
            case 7: buscar_por_categoria(lista, total_produtos);
                break;
            case 8: ordenar_e_exibir(lista, total_produtos, 1);
                break;
            case 9: ordenar_e_exibir(lista, total_produtos, 2);
                break;
            case 10: ordenar_e_exibir(lista, total_produtos, 3);
                break;
            case 11: ordenar_e_exibir(lista, total_produtos, 4);
                break;
            case 12: exportar_csv(lista, total_produtos);
                break;
            case 0: printf("Saindo do programa...\n");
                break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0); // Repete o menu até o usuário escolher 0

    return 0; // Finaliza o programa com sucesso
}
