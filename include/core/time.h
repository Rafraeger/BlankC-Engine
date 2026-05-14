#ifndef TIME_H
#define TIME_H

#ifdef __cplusplus
extern "C" {
#endif

void time_init(void);
void time_tick(void);
float time_delta(void);
float time_total(void);
int time_fps(void);

#ifdef __cplusplus
}
#endif

#endif /* TIME_H */
