# EXE3

![](imgs/exe3.gif)

Neste exercício vocês vão utilizar o RTOS para fazer dois LEDs piscarem. Os leds começam piscar quando o respectivo botão for apertado e param de piscar quando o outro botão for apertado.

**Detalhes de funcionalidade:**

- Ao apertar o botão verde referente ao led Azul, o LED azul comeca piscar (50 ms de período).
- Ao apertar o botão azul, o LED azul para de piscar.

- Ao apertar o botão verde referente ao led Amarelo, o LED Amarelo comeca piscar (50 ms de período).
- Ao apertar o botão amarelo, o LED amarelo para de piscar.


**Detalhes do firmware:**

![](imgs/exe3_diagrama.png)

- Utilizar RTOS.
- Seguir estrutura proposta do firmware.
- Utilizar período de 50 ms para piscar os LEDs.
- Deve trabalhar com interrupções nos botões.  
- Não é permitido usar:
    - `sleep_ms(), sleep_us()`.
    - Qualquer variável global que não recursos do RTOS (fila e semáforo)
- **printf** pode atrapalhar o tempo de simulação, comente antes de testar.

## Testes

O código deve passar em todos os testes para ser aceito:

- `embedded_check`
- `firmware_check`
- `wokwi`

Caso acredite que o seu código está funcionando, só que os testes falham, preencha o forms:

[Google forms para revisão manual](https://docs.google.com/forms/d/e/1FAIpQLSdikhET4iqFwkOKmgD-G6Ri-2kCdhDLndlFWXdfdcuDfPnYHw/viewform?usp=dialog)
