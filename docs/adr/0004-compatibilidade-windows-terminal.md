# Compatibilidade com Terminal Windows

Para permitir que a aplicação console desenvolvida em C seja corretamente visualizada durante a execução no sistema operacional Windows (por exemplo, quando o professor for avaliar executando direto o arquivo `.exe`), o programa precisa de um comando de pausa antes do encerramento final (ou ao final de exibições extensas).

Decidimos incluir rotinas como `system("pause")` ou leitura de um caracter (`getchar()`) estratégico ao final da execução ou nos menus para impedir que a janela do CMD ou PowerShell feche imediatamente.

Sem isso, a avaliação acadêmica seria prejudicada pois a tela desapareceria antes da leitura do output. No Linux, a interação costuma se manter no emulador de terminal hospedeiro, mas o programa precisa obrigatoriamente lidar com a limitação do Windows.
