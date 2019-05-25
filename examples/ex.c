#include <stdio.h>
#include <footag/footag.h>

static void pretty_typeinfo(const struct footag_typeinfo *t)
{
        printf("name  = %s\n", t->name);
        printf("type  = %d\n", t->type);
        printf("brief = %s\n", t->brief);
        if (t->desc) {
                printf("desc  = %s\n", t->desc);
        }
}

static void pretty_param(const struct footag_param *p) {
        printf("  name  = %s\n", p->name);
        if (p->item.type == FOOTAG_DATA_INTEGER) {
                printf("  type  = integer");
                printf(" (step=%d", p->item.data.i.step);
                printf(", min=%d", p->item.data.i.min);
                printf(", max=%d)\n", p->item.data.i.max);
                printf("  value = %d\n", p->item.data.i.val);
        } else if (p->item.type == FOOTAG_DATA_FLOAT) {
                printf("  type  = float\n");
                printf("  value = %.3f\n", p->item.data.f);
        } else if (p->item.type == FOOTAG_DATA_LENGTH) {
                printf("  type  = length\n");
                printf("  value = %.3f mm\n", p->item.data.l);
        } else if (p->item.type == FOOTAG_DATA_TOL) {
                printf("  type  = tol\n");
                printf(
                        "  value = (%.3f .. %.3f) mm\n",
                        p->item.data.t.min,
                        p->item.data.t.max
                );
        } else if (p->item.type == FOOTAG_DATA_ENUM) {
                printf("  type  = enum\n");
                const struct footag_enum *e = &p->item.data.e;
                for (unsigned int i = 0; i < e->num; i++) {
                        printf("  %c ", e->val == i ? '*' : ' ');
                        printf("%2u: ", i);
                        printf("%s\n", e->strs[i]);
                }
        }
}

static void pretty_params(const struct footag_param *p) {
        while (p->id != FOOTAG_PARAM_DONE) {
                if (p->id == FOOTAG_PARAM_TOPIC) {
                        printf("  - %s -\n", p->name);
                } else {
                        pretty_param(p);
                        puts("");
                }
                p++;
        }
}

static void pretty_spec(const struct footag_spec *s)
{
        const struct footag_rlimit *b = &s->body;
        printf(" body:  (%.3f, %.3f) (%.3f, %.3f)\n", b->minx, b->miny, b->maxx, b->maxy);
        printf(" npads: %d\n", s->npads);
        for (int i = 0; i < s->npads; i++) {
                const struct footag_pad *p = &s->pads[i];
                printf("  %4s: ", p->name);
                printf("(%.3f, %.3f) (%.3f, %.3f)", p->x, p->y, p->w, p->h);
                if (p->stack == FOOTAG_PADSTACK_SMD_RRECT) {
                        printf(" [%.3f]", p->param);
                }
                printf(" angle=%ld\n", p->angle);
        }
}

int main(void)
{
        for (int i = 0; i < FOOTAG_TYPE_NUM; i++) {
                const struct footag_typeinfo *ti = footag_get_typeinfo(i);
                if (!ti) {
                        printf("ERROR: no typeinfo for type=%d\n", i);
                        continue;
                }
                printf("- TYPEINFO -\n");
                pretty_typeinfo(ti);

                struct footag_ctx *ctx;
                const struct footag_spec *spec;
                struct footag_param *params;

                ctx = footag_open(ti->type);
                if (!ctx) {
                        puts("");
                        continue;
                }

                params = footag_get_param(ctx);
                printf(" - PARAMS -\n");
                pretty_params(params);
                /* build ui from param */
                /* set stuff in param, possibly with helpers */

                spec = footag_get_spec(ctx);
                printf(" - SPEC -\n");
                pretty_spec(spec);
                /* use the spec */

                footag_close(ctx);
                puts("");
        }
 
        return 0;
}

