
// Integrantes: Victor Silveira Portelinha, Lucas Gabriel Nunes Geremias
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "basic_io.h"
#include <stdio.h>

int motor0 = 0;
int motor1 = 0;
int motor2 = 0;
int motor3 = 0;

// Variáveis globais(sentido, direção e orientação
int sentido;
int direcao;
int orientacao;


void Arfagem(void* pvParameters);


void Guinada(void* pvParameteres);


void Rolagem(void* pvParameteres);
/*
 Define as estruturas de molde das tastks
*/

// Criando a váriavel para o semáforo
SemaphoreHandle_t xSemaphore;

void Arfagem(void* pvParameters)
{

	direcao = (int*)pvParameters;
	int count = 0;

	while (1) {


		if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
			if (direcao == 1) { // mover para frente
				vPrintString("Mover para frente\n");
				if (motor2 != 0 && motor3 != 0) {
					motor2--;
					motor3--;
				}

				motor0++;
				motor1++;
				count++;

			}
			else if (direcao == 0) { // mover para trás
				vPrintString("Mover para trás\n");
				if (motor0 != 0 && motor1 != 0) {
					motor0--;
					motor1--;
				}
				motor1++;
				motor3++;
				count++;
			}
			vPrintStringAndNumber("Motor 0: ", motor0);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 1: ", motor1);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 2: ", motor2);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 3: ", motor3);

			xSemaphoreGive(xSemaphore);
			vTaskDelay(40);
			vTaskDelay(10);
			// Quando ocorrer 25 vezes, a task é deletada, e a próxima é acionada pelo scheduler
			if (count == 25) {
				vTaskDelete(NULL);
			}
		}
	}
}

void Guinada(void* pvParameters) {

	sentido = (int*)pvParameters;
	int count = 0;

	while (1) {


		if (SemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
			if (sentido == 1) { // sentido horário
				vPrintString("Guinada no sentido horário\n");
				if (motor1 != 0 && motor3 != 0) {
					motor1--;
					motor3--;
				}

				motor2++;
				motor0++;
				count++;

			}
			else if (sentido == 0) { // mover para trás
				vPrintString("Guinada no sentido anti-hórário\n");
				if (motor2 != 0 && motor0 != 0) {
					motor0--;
					motor2--;
				}
				motor1++;
				motor3++;
				count++;
			}
			vPrintStringAndNumber("Motor 0: ", motor0);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 1: ", motor1);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 2: ", motor2);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 3: ", motor3);
			xSemaphoreGive(xSemaphore);
			vTaskDelay(40);
			vTaskDelay(10);
			// Quando ocorrer 100 vezes, a task é deletada, e a próxima é acionada pelo scheduler
			if (count == 100) {
				vTaskDelete(NULL);
			}
		}
	}


}

void Rolagem(void* pvParameters) {

	orientacao = (int*)pvParameters;
	int count = 0;

	while (1) {


		if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE) {
			if (orientacao == 1) { // rolagem direita
				vPrintString("Rolagem para a direita\n");
				if (motor1 != 0 && motor3 != 0) {
					motor1--;
					motor3--;
				}

				motor3++;
				motor0++;

				count++;

			}
			else if (orientacao == 0) { // rolagem esquerda
				vPrintString("Rolagem para a esquerda\n");
				if (motor0 != 0 && motor3 != 0) {
					motor0--;
					motor2--;
				}
				motor1++;
				motor2++;
				count++;
			}
			vPrintStringAndNumber("Motor 0: ", motor0);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 1: ", motor1);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 2: ", motor2);
			vPrintString("\n");
			vPrintStringAndNumber("Motor 3: ", motor3);
		}
		xSemaphoreGive(xSemaphore);
		vTaskDelay(20);
		// Quando ocorrer 50 vezes, a task é deletada, e a próxima é acionada pelo scheduler
		if (count == 50) {
			vTaskDelete(NULL);
		}
	}


}


int main_(void)
{
	xSemaphore = xSemaphoreCreateBinary();

	if (xSemaphore != NULL) {
		// O o *pvParameter equivale a um byte que condiz com a direção de cada task
		xTaskCreate(Arfagem, "arfagem", 1000, (void*)1, 1, NULL);

		xTaskCreate(Rolagem, "rolagem", 1000, (void*)0, 1, NULL);

		xTaskCreate(Guinada, "guinada", 1000, (void*)1, 1, NULL);

		// Inicia o escalonador de tarefas
		vTaskStartScheduler();
	}
	else {

	}
	for (;; );
	return 0;
}
