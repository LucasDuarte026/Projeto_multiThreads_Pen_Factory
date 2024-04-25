# Projeto Caneta - Simulação de uma Fábrica de Canetas em C com Pthreads

## Descrição do Projeto

Este projeto consiste em uma aplicação em C utilizando a biblioteca Pthreads para simular uma fábrica de canetas, com ênfase na utilização de multithreading, semáforos, variáveis de condição e mutex. A simulação ocorre em um ambiente Linux e é executada por um total de seis threads, cada uma desempenhando um papel específico na fábrica. As threads interagem entre si nas atividades de fabricação de canetas, armazenamento de matéria-prima e canetas produzidas, e compra/venda.

## Integrantes do Grupo

- Lucas Sales Duarte (11734490)
- Daniel Filho (13677114)
- Daniel Umeda (13676541)
- Manoel Thomaz (13676392)

## Funcionamento da Aplicação

A função `main()` serve como ponto de entrada da aplicação e é executada pela thread de rank 0 (Criador), que recupera os argumentos de entrada e chama a função `criador()`. As demais threads, com rank > 0, iniciam a execução diretamente em suas respectivas funções.

Os argumentos de entrada configuram a execução da aplicação e incluem informações como a quantidade de matéria-prima no depósito, a quantidade de unidades enviadas pelo Depósito de Matéria Prima à célula de fabricação, tempos de espera, e outras configurações relevantes.

As threads são sincronizadas por semáforos e variáveis de condição onde necessário. Por exemplo, a thread de Controle (rank 3) determina se o Depósito de Matéria Prima pode transferir matéria-prima para a Célula de Fabricação de Canetas e se esta última pode produzir canetas.

O Depósito de Canetas (rank 4) tem capacidade limitada de armazenamento. Quando sua capacidade máxima é atingida, o envio de matéria-prima, a fabricação e o envio de canetas ao depósito devem ser suspensos.

O Comprador (rank 5) iterativamente solicita a compra de canetas e o depósito, se possível, as envia na quantidade especificada. Se não houver canetas suficientes em estoque, o comprador é informado.

A aplicação é encerrada quando todo o estoque de matéria-prima acaba e não há mais canetas produzidas no depósito de canetas.

## Documentação Técnica

O projeto utiliza conceitos avançados de multithreading, incluindo o uso de semáforos, variáveis de condição e mutex para garantir a sincronização adequada entre as threads. A manipulação desses recursos é crucial para garantir o funcionamento correto da fábrica de canetas, evitando condições de corrida e garantindo o acesso seguro aos recursos compartilhados.

A figura em anexo apresenta uma descrição básica da aplicação em Redes de Petri, destacando os pontos de interação entre as threads e as transições de estado.

## Observações

- Este projeto foi desenvolvido como parte do curso de Engenharia de Computação na Universidade de São Paulo (USP) - São Carlos.
- O código-fonte completo da aplicação e mais detalhes técnicos podem ser encontrados nos arquivos fornecidos.
- Qualquer dúvida ou sugestão de melhoria, sinta-se à vontade para entrar em contato com o grupo de desenvolvimento.
