#ifndef __HDMA_DEV_H
#define __HDMA_DEV_H

enum hdma_state {
        HDMA_IS_ON = 0,
        HDMA_IS_OFF,
        HDMA_IS_MIGRATING,
};

enum hdma_state get_hdma_status(void);
int set_hdma_status(enum hdma_state state);

#endif
