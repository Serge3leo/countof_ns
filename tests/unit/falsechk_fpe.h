// vim:set sw=4 ts=8 et fileencoding=utf8:
int fc_fpe_x = 1;
int fc_fpe_z = 0;
size_t TU_UNIT(void) {
    return fc_fpe_x/fc_fpe_z;
}
