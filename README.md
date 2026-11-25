# PS4 LIVE — protótipo homebrew

**Alvo informado:** PS4 firmware 13.52 / GoldHEN v2.4b18.11  
**Plataforma de live escolhida:** Twitch  
**Tema:** roxo + preto  
**Resolução desejada:** 1920×1080 (ainda não validada para captura/codificação no console)

## Estado desta entrega

Esta é a **base inicial do projeto**, não um transmissor funcional ainda.

- [x] Estrutura C++/SDL2 para homebrew PS4 usando OpenOrbis Toolchain
- [x] Interface inicial roxa/preta com estado de transmissão explícito
- [x] Metadados básicos do pacote
- [ ] Captura de gameplay do sistema
- [ ] Codificação H.264 por hardware/software
- [ ] Envio RTMP à Twitch e tratamento de reconexão
- [ ] Teste de desempenho e estabilidade no hardware real
- [ ] PKG compilado e validado no console

A interface não exibe “LIVE” nem simula conexão: enquanto a captura/RTMP não existirem, mostra **MÓDULO DE CAPTURA PENDENTE**.

## Firmware e GoldHEN

A foto do console mostra `HEN 13.52` e `GoldHEN v2.4b18.11`. Vamos tratar 13.52 como alvo de compilação/teste, mas confirme em **Configurações → Sistema → Informações do sistema** se o campo de software do sistema realmente indica 13.52. Não atualize o console para este projeto.

## Compilação

A compilação exige Linux ou macOS com OpenOrbis PS4 Toolchain instalado. O desenvolvimento dos arquivos pode ser feito no celular, mas o PKG precisa ser gerado por uma máquina/runner compatível.

1. Instale a OpenOrbis PS4 Toolchain seguindo o README oficial: https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain
2. Configure `OO_PS4_TOOLCHAIN` apontando para a pasta da toolchain.
3. Garanta que SDL2 e os módulos exigidos estejam disponíveis na toolchain.
4. Na pasta deste projeto, execute `make`.

O Makefile usa as ferramentas `create-fself`, `create-gp4` e `PkgTool.Core` fornecidas pela toolchain. Este scaffold ainda precisa ser testado em um ambiente real de compilação; não considere o PKG pronto até a compilação terminar sem erros e o pacote ser testado no console.

## Estrutura

```text
PS4_LIVE/
├── Makefile
├── README.md
├── src/main.cpp
├── assets/                 # recursos do app
├── sce_sys/                # ícone e metadados do pacote
└── preview.html            # prévia visual no navegador
```

## Segurança da chave Twitch

Não coloque sua chave de transmissão em código-fonte, HTML, prints ou no repositório. A futura implementação deverá guardá-la localmente no console e nunca enviá-la a terceiros. Para testes iniciais, use uma chave de transmissão secundária e revogue-a se for exposta.

## Próxima fase técnica

Antes de implementar RTMP, precisamos provar que o homebrew consegue obter frames de gameplay de forma suportada/estável no firmware-alvo. SDL2 aqui é apenas a camada de interface; não captura por si só a imagem do jogo. Depois disso, selecionaremos encoder e biblioteca de transporte compatíveis com a toolchain.
