#include "timer.h"

/* timerX_isr
 *   timerX ISR (Interrupt Service Routine)
 */
static OnTick callback2=NULL;
static OnTick callback3=NULL;
static OnTick callback4=NULL;
static OnTick callback5=NULL;

__attribute__ ((weak))
void TIM2_IRQHandler(void) {
	// call callback it is defined
	callback2();	
	// acknowledge IRQ
	_TIM2->SR &= ~0x1F;
}

void TIM3_IRQHandler(void) {
	// call callback it is defined
	callback3();

	// acknowledge IRQ
	_TIM3->SR &= ~0x1F;
}

void TIM4_IRQHandler(void) {
	// call callback it is defined
	callback4();
	// acknowledge IRQ
	_TIM4->SR &= ~0x1F;
}

void TIM5_IRQHandler(void) {
	// call callback it is defined
	callback5();
	// acknowledge IRQ
	_TIM5->SR &= ~0x1F;
}

/* timer_wait_ms
 *   wait for ms millisecond function
 */
int timer_wait_ms(TIM_t *tmr, uint32_t ms)
{
	uint32_t clk;

	if (tmr == _TIM2) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1;
	} else if (tmr == _TIM3) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<1;
	} else if (tmr == _TIM4) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<2;
	} else if (tmr == _TIM5) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<3;
	} else {
		return -1;
	}
	
	// set mode: auto reload, preload enabled + One Pulse Mode
	tmr->CR1 =  (1<<7) | (1<<3);
	
	// set prescaler 100us
	tmr->PSC = clk/10000-1;
	
	// set period
	tmr->ARR = ms*10-1;
	
	// poll for timeout
	// force update to reset counter and prescaler
	tmr->EGR = 1;

	// enable counting
	tmr->CR1 |= 1;
	
	// wait for timeout
	while (tmr->CR1 & 1);
	
    return 0;
}

/* timer_wait_us
 *   wait for us microsecond function
 */
int timer_wait_us(TIM_t *tmr, uint32_t us)
{
	uint32_t clk;
	
	if (tmr == _TIM2) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1;
	} else if (tmr == _TIM3) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<1;
	} else if (tmr == _TIM4) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<2;
	} else if (tmr == _TIM5) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<3;
	} else {
		return -1;
	}
	
	// set mode
	tmr->CR1 =  (1<<7) | (1<<3);
	
	// set prescaler 1us
	tmr->PSC = clk/1000000-1;
	
	// set period
	tmr->ARR = us-1;
	
	// poll for timeout
	// force update to reset counter and prescaler
	tmr->EGR = 1;

	// enable counting
	tmr->CR1 |= 1;
	
	// wait for timeout
	while (tmr->CR1 & 1);
	
    return 0;
}

/* timer_tick_init
 *   setup timer to call cb function periodically, each tick_ms
 */
int timer_tick_init(TIM_t *tmr, uint32_t tick_ms, OnTick cb)
{
	IRQn_t	 irqn;
	uint32_t irq_priority, clk;
	clk = sysclks.apb1_timer_freq;
		
	if (tmr == _TIM2) {
		// register callback function
		callback2=cb;	  //voir en tête de fichier
		// enable timer clocking
		_RCC->APB1ENR |=0x1;	
	} else if (tmr == _TIM3) {
		// register callback function
		callback3=cb;
		// enable timer clocking
		_RCC->APB1ENR |=0x1<<1;
	} else if (tmr == _TIM4) {
		// register callback function
		callback4=cb;
		// enable timer clocking
		_RCC->APB1ENR |=0x1<<2;
	} else if (tmr == _TIM5) {
		// register callback function
		callback5=cb;
		// enable timer clocking
		_RCC->APB1ENR |=0x1<<3;
	} else {
		return -1;
	}
	
	// clear pending interrupts
	tmr->SR &= ~0x1F;
	
	// set mode
	tmr->CR1=1<<7;
	// set prescaler 100us
	tmr->PSC=sysclks.apb1_timer_freq/10000-1;
	// set period
	tmr->ARR=tick_ms*10-1;
	//int _priority=4;
	if (cb) {  // NULL ou adresse
		if(tmr==_TIM2){
			irqn=TIM2_IRQn;
			irq_priority = 5;
		}else if(tmr==_TIM3){
			irqn=29;
			irq_priority = 6;
		}else if(tmr==_TIM4){
			irqn=30;
			irq_priority = 7;
		}else if(tmr==_TIM5){
			irqn=50;
			irq_priority = 8;
		}else{
			return -1;
		}
	
		// configure NVIC
		NVIC_SetPriority(irqn, irq_priority ); //voir include/cmsis/core_cm4.h & include/config.h
	    NVIC_EnableIRQ(irqn );

		// enable timer irq
		tmr->DIER =1;
	}
	
    return 0;
}

/* timer_tick_period
 *   change the tick_ms period
 */
int timer_tick_period(TIM_t *tmr, uint32_t tick_ms)
{
    // set period
    tmr->ARR=tick_ms*10-1;
    // force update to reset counter and prescaler
    tmr->EGR = 1;	
	return 0;
}

/* timer_start
 *   reset & enable counting
 */
void timer_start(TIM_t *tmr)
{
	// force update to reset counter and prescaler
        tmr->EGR = 1;
        // start counting
        tmr->CR1 |= 1;
}

/* timer_stop
 *   stop counting
 */
void timer_stop(TIM_t *tmr)
{
	// disable counting
	tmr->CR1&=~(0x1);
	
}

/* timer_read
 *   return the counter value
 */
uint32_t timer_read(TIM_t *tmr)
{
    return tmr->CNT;
}


/* pwm_init
 *   setup timer with period_ms period
 */
int pwm_init(PWM_t *tmr, uint32_t period_us)
{
	uint32_t clk;
	
	if (tmr == _TIM2) {		
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1;
	} else if (tmr == _TIM3) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<1;
	} else if (tmr == _TIM4) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<2;
	} else if (tmr == _TIM5) {
		// enable timer clocking
		clk = sysclks.apb1_timer_freq;
		_RCC->APB1ENR |= 1<<3;
	} else {
		return -1;
	}
	
	// clear pending interrupts
	tmr->SR &= ~0x1F;
	
	// set mode
	tmr->CR1 = (1<<7);
	
	// set prescaler 1us
	tmr->PSC = clk/1000000-1;
	
	// set period
	tmr->ARR = period_us-1;
	
    return 0;
}

/* pwm_channel_enable
 *   set up pwm channel
 */
int pwm_channel_enable(PWM_t *pwm, uint32_t channel, uint32_t dutycycle, uint32_t oe)
{
	if (channel>3 || dutycycle>100) return 0;
	
	// set "on" time
	pwm->CCR[channel] = (pwm->ARR+1)*dutycycle/100;
	
	// OCM_PWM1 | OCPE
	pwm->CCMR[channel>>1] = (pwm->CCMR[channel>>1] & ~(0xFF << ((channel & 1)<<3))) | (((6<<4) | (1<<3)) << ((channel & 1)<<3)); //(0110100000000000)
	
	if (oe) {
		pwm->CCER = (pwm->CCER & ~(0xF<<(channel<<2))) | (1 << (channel<<2));
	} else {
		pwm->CCER = (pwm->CCER & ~(0xF<<(channel<<2)));
	}
	
	return 1;
}


/* pwm_channel_disable
 *   disable pwm channel
 */
int pwm_channel_disable(PWM_t *pwm, uint32_t channel)
{
	pwm->CCER = (pwm->CCER & ~(0xF<<(channel<<2)));
	return 1;
}


/* pwm_channel_set
 *   set up dutycycle for pwm channel
 */
int pwm_channel_set(PWM_t *pwm, uint32_t channel, uint32_t dutycycle)
{
	if (channel>3 || dutycycle>100) return 0;
	
	// set "on" time
	pwm->CCR[channel] = (pwm->ARR+1)*dutycycle/100;

	return 1;	
}