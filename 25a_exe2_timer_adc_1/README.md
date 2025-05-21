# EXE2

![](imgs/exe2.gif)

Neste exercício, vocês utilizarão o periférico de **timer** para piscar o LED. O período na qual o LED pisca será definido por uma valor lido do **ADC** no potênciometro seguindo as zonas a seguir:

- 0.0V até 1.0V: Desligado
- 1.0V até 2.0V: 300 ms
- 2.0V até 3.3V: 500 ms

![](imgs/zones.png)

**Detalhes de funcionalidade:**

- Ler o ADC e verificar a zona
- Manter o LED piscando com o período definido pela zona
- O LED deve apagar sempre que estiver na zona 0

**Detalhes do firmware:**

> ⚠️ ⚠️ O wokwi está com um bug quando cancelamos um timer e precisamos criar outro logo na sequência.
> Para resolver isso temporariamente, sugerirmos que vocês **não cancelem e criem um novo timer**. Trabalhem
> com dois timers! Cada um operando nas diferentes frequências.

- Baremetal (sem RTOS).
- Deve trabalhar com **TIMER e ADC**.
- Deve trabalhar com interrupções nos botões.  
- Não é permitido usar `sleep_ms(), sleep_us()`.
- **printf** pode atrapalhar o tempo de simulação, comenta/remova antes de testar.

## Testes

O código deve passar em todos os testes para ser aceito:

- `embedded_check`
- `firmware_check`
- `wokwi`

Caso acredite que o seu código está funcionando, só que os testes falham, preencha o forms:

[Google forms para revisão manual](https://docs.google.com/forms/d/e/1FAIpQLSdikhET4iqFwkOKmgD-G6Ri-2kCdhDLndlFWXdfdcuDfPnYHw/viewform?usp=dialog)

