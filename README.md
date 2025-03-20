# Tutorial: Análise Estática com *clang-tidy* e Lint para MISRA-C
Este projeto tem como objetivo explicar como realizar análise estática do código utilizando o clang-tidy, incluindo regras de lint para MISRA-C, tomando como exemplo o projeto de uma calculadora simples em C. A estrutura do projeto é a seguinte:

```
.
├── Makefile
├── README.md
├── .clang-tidy
├── .gitignore
└── src
    ├── calculadora.c
    └── calculadora.h
```

[![MISRA-C Linting with Clang-Tidy](https://github.com/EmanuelAdler/tutorial-clang-tidy/actions/workflows/linting-dev.yml/badge.svg?branch=develop)](https://github.com/EmanuelAdler/tutorial-clang-tidy/actions/workflows/linting-dev.yml)

## 1. O que é clang-tidy?
O clang-tidy é uma ferramenta de análise estática de código e lint, parte do ecossistema LLVM/Clang. Ele ajuda a identificar possíveis problemas, bugs, violações de padrões de codificação, entre outros. A vantagem do clang-tidy é a grande variedade de checks disponíveis, incluindo verificações para MISRA-C (um conjunto de regras de codificação seguro e confiável para C).

## 2. O arquivo `.clang-tidy`
Neste projeto, há um arquivo chamado .clang-tidy que configura quais regras e verificações devem ser aplicadas. Um exemplo de configuração pode ser:

```yaml
Checks: >
  -*,
  misra-c2012-*,
  readability-*,
  bugprone-*,
  performance-*
WarningsAsErrors: "*"
```

### Significado dos campos

- **Checks**: define quais verificações serão habilitadas ou desabilitadas.  
  - `-*,` desabilita todos os checks por padrão.  
  - `misra-c2012-*` habilita todas as regras de MISRA C 2012.  
  - `readability-*`, `bugprone-*`, `performance-*` habilitam grupos de verificações relacionadas a legibilidade, problemas potenciais de *bugs* e questões de desempenho, respectivamente.  
- **WarningsAsErrors**: indica que todos os avisos devem ser tratados como erros (`"*"` significa todos).

Essa configuração pode ser ajustada conforme as necessidades do projeto. Para ver a lista completa de *checks* suportados, consulte a [documentação oficial do clang-tidy](https://clang.llvm.org/extra/clang-tidy/).

## 3. Pré-requisitos

1. **clang-tidy** instalado.  
   - Em distribuições baseadas em Debian/Ubuntu, você pode instalar com:  
     ```bash
     sudo apt-get update
     sudo apt-get install clang-tidy
     ```
   - Em outras distribuições, procure o pacote correspondente ou instale via [fonte oficial do LLVM](https://releases.llvm.org/).

2. (Opcional) **Bear** para gerar `compile_commands.json` automaticamente.  
   - O clang-tidy funciona melhor quando tem acesso ao *compilation database*, ou seja, um arquivo `compile_commands.json` que descreve como cada arquivo do projeto é compilado.  
   - O [Bear](https://github.com/rizsotto/Bear) é uma ferramenta que intercepta as chamadas ao compilador durante o `make` e gera esse arquivo.

## 4. Gerando o arquivo `compile_commands.json` (opcional, mas recomendado)

Quando seu projeto não utiliza CMake (como é o caso aqui, que usa um Makefile simples), você pode usar o Bear para criar o `compile_commands.json`.

1. Instale o Bear:
   ```bash
   sudo apt-get install bear
   ```
2. Na pasta raiz do seu projeto (onde está o `Makefile`), execute:
   ```bash
   make clean
   bear -- make
   ```
   - O `make clean` garante que você force a recompilação de todos os arquivos.
   - O comando `bear -- make` irá compilar o projeto e, simultaneamente, gerar um arquivo `compile_commands.json` na raiz do projeto.

Verifique se o arquivo `compile_commands.json` foi criado com sucesso. Ele deve conter as linhas de compilação de cada arquivo `.c` do seu projeto.

## 5. Executando o clang-tidy

Existem duas formas principais de executar o clang-tidy:

### 5.1. Usando o `compile_commands.json`

Com o `compile_commands.json` gerado, você pode rodar:

```bash
clang-tidy src/calculadora.c -p .
```

- `-p .` indica para o clang-tidy que o arquivo de *compilation database* está na pasta atual (caso ele esteja em outro local, ajuste o caminho).
- Você pode analisar um arquivo específico (como `src/calculadora.c`) ou vários arquivos (`src/*.c`).

Para verificar todos os arquivos `.c` de uma só vez:

```bash
clang-tidy src/*.c -p .
```

Os arquivos header (`.h`) devem ser analisados, mas geralmente são verificados indiretamente quando incluídos em arquivos fonte (`.c` ou `.cpp`). Isso significa que, ao rodar ferramentas como o clang-tidy em seus arquivos de implementação, o conteúdo dos headers também é avaliado durante a compilação. Se um header não for incluído em nenhum arquivo fonte, ele pode acabar sem ser analisado. Nesse caso, você pode executá-lo explicitamente com a ferramenta se necessário.

### 5.2. Sem usar `compile_commands.json`

Caso não queira ou não possa usar o Bear para gerar o *compilation database*, você pode fornecer manualmente as flags de compilação para cada arquivo. Por exemplo:

```bash
clang-tidy src/calculadora.c -- \
    -I./src \
    -std=c11 \
    -Wall -Wextra
```

- Após o `--`, você passa as mesmas opções que usaria ao compilar o arquivo (incluindo cabeçalhos e definições de macros).

Entretanto, **recomenda-se** utilizar o *compilation database* sempre que possível, pois evita ter que lembrar e repetir manualmente todas as opções de compilação.

## 6. Interpretando os resultados

- O clang-tidy exibirá mensagens de alerta/erro no terminal, indicando a linha do arquivo em que um problema foi encontrado e uma descrição do problema.
- Como definimos `WarningsAsErrors: "*"`, qualquer *warning* será tratado como erro. Se você achar que está recebendo erros demais, pode ajustar essa configuração para filtrar ou reduzir a severidade.

### Exemplo de saída

Suponha que haja um problema de possível *overflow* ou violação de regra MISRA. A saída pode ser algo como:

```
src/calculadora.c:10:5: error: ...
  ...
```

- `src/calculadora.c:10:5` indica o local (linha e coluna) no arquivo.
- `error:` indica o tipo de mensagem (nesse caso, um *warning* promovido a erro).
- A descrição detalhada informará qual regra ou qual *check* foi violado.

## 7. Integração com GitHub Actions

Você pode automatizar a análise estática do código usando o clang-tidy via GitHub Actions. Para isso, siga as instruções abaixo.

### Configuração do workflow

Crie um arquivo em `.github/workflows/linting.yml` com o conteúdo abaixo:

```yaml
name: MISRA-C Linting with Clang-Tidy

on:
  push:
    branches:
      - main

jobs:
  lint:
    runs-on: ubuntu-latest

    steps:
      - name: Checkout Code
        uses: actions/checkout@v3

      - name: Install Dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y make clang-tidy bear

      - name: Generate compile_commands.json with Bear
        run: |
          make clean
          bear -- make

      - name: Run Clang-Tidy on Source Files
        run: clang-tidy src/*.c -p .
```

### Como funciona?

- **Acionamento (Trigger):**  
  O workflow é disparado em cada push na branch `main`.

- **Etapas do Workflow:**  
  1. **Checkout Code:** Faz o checkout do repositório, garantindo que o arquivo `.clang-tidy` (já versionado) esteja presente.  
  2. **Install Dependencies:** Instala o Make, clang-tidy e o Bear.  
  3. **Generate compile_commands.json with Bear:** Executa o comando `make clean` e, em seguida, utiliza o Bear para gerar o arquivo `compile_commands.json`, que é necessário para o clang-tidy saber como compilar os arquivos.  
  4. **Run Clang-Tidy on Source Files:** Roda o clang-tidy analisando todos os arquivos C na pasta `src/`, utilizando as configurações definidas no arquivo `.clang-tidy`.

Com essa configuração, o seu projeto passará por uma análise estática a cada push na branch `main`, garantindo que as regras MISRA-C e demais verificações sejam aplicadas automaticamente.

## 8. Dicas de boas práticas

- **Mantenha o arquivo `.clang-tidy` versionado** no repositório, para que todos os colaboradores usem as mesmas regras de verificação.
- **Integre o clang-tidy no seu fluxo de CI/CD**: é comum configurar pipelines para rodar clang-tidy a cada *push* ou *merge request*, garantindo que o código atenda aos padrões de qualidade e segurança.
- **Não ignore os avisos**: mesmo que sejam apenas *warnings*, podem indicar problemas reais que levarão a *bugs* ou comportamentos indesejados no futuro.

## 9. Conclusão

Usar o clang-tidy com as regras MISRA-C é uma excelente forma de melhorar a qualidade, segurança e confiabilidade do seu código C. Seguindo os passos acima, você terá:

1. Instalado o clang-tidy (e opcionalmente o Bear).
2. Configurado o arquivo `.clang-tidy` com as regras desejadas (MISRA, readability, bugprone, etc.).
3. Gerado o `compile_commands.json` (usando Bear) ou fornecido manualmente as flags de compilação.
4. Executado a análise com `clang-tidy` e interpretado os resultados.
5. Integrado a análise automática com `clang-tidy` via Github Actions a cada push na branch main.

Com isso, você terá uma base sólida para evoluir seu projeto de calculadora (ou qualquer outro em C) mantendo boas práticas de codificação e conformidade com padrões de segurança.
