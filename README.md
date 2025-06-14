# # 💧 Captação Inteligente de Água da Umidade do Ar com IoT

Este projeto simula um sistema IoT capaz de captar e monitorar a umidade atmosférica com objetivo de gerar água potável em ambientes domésticos. O sistema utiliza sensores para leitura de temperatura e umidade, atuadores simulando o funcionamento do equipamento, comunicação em tempo real via protocolo MQTT e visualização de dados através do Node-RED.

---

## 📌 Descrição do funcionamento

O sistema proposto realiza:

1. **Leitura de sensores DHT22 e DS18B20** para coletar temperatura ambiente, umidade relativa do ar e temperatura de uma superfície fria (condensadora).
2. **Decisão automática** com base em parâmetros ambientais (ex: umidade > 70% e temperatura < 25°C).
3. **Ativação de atuadores simulados** (LED e resistor) para representar o início do processo de condensação.
4. **Envio dos dados via protocolo MQTT** para um broker público (HiveMQ).
5. **Visualização em tempo real no Node-RED Dashboard**, com gráficos de temperatura e umidade.

A simulação foi realizada na plataforma [Wokwi](https://wokwi.com), facilitando a reprodução sem necessidade de hardware físico.

---

## 💻 ii) Software desenvolvido e documentação do código

O código principal está localizado em  [`src/projeto.cpp`](src/projeto.cpp) e foi implementado para rodar em ESP32. Ele realiza:

- Conexão Wi-Fi usando a biblioteca `WiFi.h`
- Conexão com o broker HiveMQ via `PubSubClient.h`
- Leitura de sensores:
  - `DHT22`: temperatura e umidade
  - `DS18B20`: temperatura da superfície fria
- Publicação dos dados via tópicos MQTT:
  - `meuProjeto/temperatura`
  - `meuProjeto/umidade`
  - `meuProjeto/tempFria`
- Subscrição ao tópico `meuProjeto/led` para controle remoto de LED
- Reconexão automática com o Wi-Fi e broker MQTT em caso de falha

O código está totalmente comentado para facilitar o entendimento e a adaptação.

---

## 🧱 Hardware utilizado (simulado no Wokwi)

| Componente       | Descrição                                                                 |
|------------------|---------------------------------------------------------------------------|
| ESP32            | Microcontrolador principal com suporte Wi-Fi e MQTT                       |
| Sensor DHT22     | Sensor digital de temperatura e umidade do ar (GPIO 21)                   |
| Sensor DS18B20   | Sensor digital de temperatura de superfície fria (GPIO 19)                |
| Resistor         | Atuador que simula a geração de calor (GPIO 18)                           |
| LED              | Indicador visual do status de ativação do sistema (GPIO 5)                |

**Prototipagem**:  
Todo o sistema foi desenvolvido e testado virtualmente na plataforma [Wokwi](https://wokwi.com), eliminando a necessidade de montagem física durante a simulação.

---

### 🧠 Interfaces utilizadas:

- **GPIOs digitais**:
  - Leitura dos sensores
  - Escrita nos atuadores (LED, resistor)

### 📡 Protocolos e módulos de comunicação:

| Protocolo | Função                                                                 |
|----------|-------------------------------------------------------------------------|
| **Wi-Fi (TCP/IP)** | Comunicação com a internet para transmissão remota              |
| **MQTT**  | Publicação/assinatura de tópicos para troca de mensagens com broker     |
| **OneWire** | Comunicação com o sensor DS18B20 (temperatura da superfície fria)      |

### 🔌 Tópicos MQTT utilizados:

| Tópico                | Função                             |
|------------------------|------------------------------------|
| `meuProjeto/led`       | Controle remoto do LED via MQTT    |
| `meuProjeto/temperatura` | Publica a temperatura do DHT22     |
| `meuProjeto/umidade`     | Publica a umidade do DHT22         |
| `meuProjeto/tempFria`    | Publica a temperatura do DS18B20   |

### 🖥️ Broker utilizado:
- HiveMQ Public Broker: `broker.hivemq.com`
- Porta: `1883`

### 🛠️ Visualização dos dados:
- **Node-RED**: Fluxos de processamento MQTT, dashboards interativos com:
  - Tabelas para temperatura e umidade
  - Indicadores de status do sistema

---

## 🌍 Comunicação via Internet (TCP/IP) com uso de MQTT

Todo o projeto depende de comunicação em rede:

- **Wi-Fi**: O ESP32 se conecta à internet utilizando uma rede Wi-Fi local.
- **Protocolo TCP/IP**: Utilizado como camada de transporte para o protocolo MQTT.
- **MQTT**: Responsável pela troca de mensagens com o broker HiveMQ, garantindo:
  - Envio dos dados dos sensores
  - Recebimento de comandos para controle do sistema
  - Monitoramento remoto no Node-RED Dashboard

Este modelo de comunicação leve e confiável é essencial para aplicações de Internet das Coisas (IoT), com suporte a ambientes com baixa largura de banda e necessidade de resposta em tempo real.

---

## 🧪 Resultados da Simulação

- Dados dos sensores foram lidos corretamente e publicados em tópicos MQTT.
- O Node-RED exibiu os dados em tempo real no dashboard.
- O LED foi acionado de forma remota com sucesso via mensagens MQTT.
- Toda a lógica de controle e comunicação foi validada na simulação virtual.

---

## 👩‍💻 Autora

- **Andrezza dos Santos Batista**
Projeto acadêmico desenvolvido em 2025 para Universidade Presbiteriana Mackenzie 

