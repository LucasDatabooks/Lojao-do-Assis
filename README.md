# Lojao-do-Assis
Unir os conceitos de programação em C para gerar documentos, alterar dados e organiza-los de forma rápida e eficaz


vamos unir os conceitos de algoritmos de ordenação com manipulação de arquivos. Utilize estrutura de dados e ponteiros. A ideia é criar um único programa completo, que crie um cadastro genérico de uma loja, com tabela de dados em texto e um programa em C completo para:

Geração automática de ID (incremental, sem precisar digitar)
Verificação de duplicatas (produto com o mesmo nome não é cadastrado duas vezes)
Funções completas: criar, listar, inserir, alterar, excluir, buscar por nome e categoria
·        Ordenar produtos por nome (A–Z)
·        Ordenar produtos por nome (Z–A)
·        Ordenar produtos por preço (menor → maior)
·        Ordenar produtos por preço (maior → menor)
Comentários explicativos linha a linha
 

Comentários:

·       Ao alterar/excluir, o programa cria um arquivo temporário e depois substitui o original — técnica comum para evitar corrupção de dados.

·       Campos têm tamanhos limitados (ex.: produto[50]) — aumente se sua aplicação precisar armazenar textos maiores.

·       Adicionar verificação de IDs duplicados na inserção;

·       Gerar um ID automático (incremental) em vez de solicitar ao usuário;

·       Exportar os dados para CSV com vírgula.

·       Comparações e buscas são “case-insensitive”.

·       Armazena em um vetor de estruturas (Produto lista[]);

·       Usa o método de ordenação Bubble Sort (simples e didático);

·       Exibe o resultado em formato de tabela sem alterar o arquivo.


