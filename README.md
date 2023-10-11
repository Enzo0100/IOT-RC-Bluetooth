# Carrinho de Controle Remoto com IoT

## Descrição
Este projeto visa desenvolver um carrinho controlado remotamente através de Bluetooth utilizando o módulo ESP32. Através da modulação PWM (Pulse Width Modulation), conseguimos controlar a velocidade e a direção dos motores do carrinho, além de uma buzina utilizando um buzzer.

## Componentes:
- **ESP32**: Controlador central que receberá os comandos via Bluetooth e acionará os motores e a buzina.
- **Motores DC**: Para movimentar o carrinho.
- **Driver para Motores (por exemplo, L293D)**: Controla os motores DC usando os sinais PWM vindos do ESP32.
- **Módulo Bluetooth (integrado ao ESP32)**: Recebe comandos do dispositivo controlador.
- **Buzzer**: Emite sons, simulando a buzina do carrinho.
- **Bateria**: Para alimentar o ESP32, motores e buzzer.
- **Chassi do carrinho**: Estrutura para montagem dos componentes.

## Funcionalidades:
1. Movimento para Frente: O carrinho se move para frente.
2. Movimento para Trás: O carrinho se move para trás.
3. Virar à Esquerda: O carrinho vira à esquerda.
4. Virar à Direita: O carrinho vira à direita.
5. Parar: O carrinho para completamente.
6. Buzina: O carrinho emite um som através do buzzer, simulando uma buzina.

## Como usar:
1. **Montagem**: Siga o esquemático anexo para montar todos os componentes no chassi.
2. **Programação**: Carregue o código fornecido no ESP32 usando a IDE do Arduino ou outra de sua preferência.
3. **Controle**: Emparelhe seu dispositivo móvel com o módulo Bluetooth do ESP32. Use o aplicativo sugerido (ou qualquer aplicativo de controle Bluetooth) para enviar comandos ao carrinho.

## Configurações iniciais

## IDE do Arduino

Baixa a ide do arduino

```
https://www.arduino.cc/en/software
```

## Adicionando repositorio na IDE 

```
https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json
```

## Configurando a placa correta para utilizar a biblioteca Bluepad32

```
https://github.com/ricardoquesada/bluepad32/blob/main/docs/plat_arduino.md
```

