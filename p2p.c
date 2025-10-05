#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Subscriber {
    char name[50];
    void (*callback)(const char *topic, const char *message, const char *name);
} Subscriber;

typedef struct Topic {
    char name[50];
    Subscriber **subscribers; 
    int count;
} Topic;

void subscribe(Topic *topic, Subscriber *sub) {
    topic->subscribers = realloc(topic->subscribers, (topic->count + 1) * sizeof(Subscriber *));
    topic->subscribers[topic->count] = sub;
    topic->count++;
    printf("%s se inscreveu no topico %s\n", sub->name, topic->name);
}

void unsubscribe(Topic *topic, Subscriber *sub) {
    for (int i = 0; i < topic->count; i++) {
        if (topic->subscribers[i] == sub) {
            for (int j = i; j < topic->count - 1; j++) {
                topic->subscribers[j] = topic->subscribers[j+1];
            }
            topic->count--;
            topic->subscribers = realloc(topic->subscribers, topic->count * sizeof(Subscriber *));
            printf("%s se descadastrou do topico %s\n", sub->name, topic->name);
            return;
        }
    }
}

void publish(Topic *topic, const char *message) {
    printf("\n[Publicando em %s]: %s\n", topic->name, message);
    for (int i = 0; i < topic->count; i++) {
        topic->subscribers[i]->callback(topic->name, message, topic->subscribers[i]->name);
    }
}

void printMessage(const char *topic, const char *message, const char *name) {
    printf("-> %s recebeu mensagem no topico %s: %s\n", name, topic, message);
}
int main() {
    Topic esportes = {"Esportes", NULL, 0};
    Topic musica = {"Musica", NULL, 0};

    Subscriber joao = {"Joao", printMessage};
    Subscriber maria = {"Maria", printMessage};
    Subscriber ana   = {"Ana", printMessage};

    subscribe(&esportes, &joao);
    subscribe(&esportes, &maria);
    subscribe(&musica, &ana);

    publish(&esportes, "Final da Copa amanha!");
    publish(&musica, "Novo album lancado!");

    unsubscribe(&esportes, &maria);

    publish(&esportes, "Resultado do jogo de ontem.");

    free(esportes.subscribers);
    free(musica.subscribers);

}
