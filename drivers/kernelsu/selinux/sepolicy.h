#ifndef __KSU_H_SEPOLICY
#define __KSU_H_SEPOLICY

#include <linux/types.h>
#include <linux/version.h>

#include "ss/policydb.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0)
/*
 * Linux 4.19 keeps the policy database embedded in struct selinux_ss and has
 * no struct selinux_policy at all. The shared policy-editing code only needs
 * the embedded database, so provide a stand-in type for pre-5.7 kernels.
 * Policies are installed through security_load_policy() there, not by
 * swapping a selinux_policy pointer.
 */
struct selinux_policy {
    struct policydb policydb;
    struct sidtab *sidtab;
    u32 latest_granting;
};
#endif

struct selinux_policy *ksu_dup_policydb(struct policydb *old_db);

// Serialize a policy database into data (size bytes). Returns 0 and stores the
// used length in *written, or a negative errno.
int ksu_write_policydb(struct policydb *db, void *data, size_t size, size_t *written);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0)
struct selinux_policy *ksu_dup_sepolicy(struct selinux_policy *old_pol);
#endif

void ksu_destroy_sepolicy(struct selinux_policy *orig);

// Operation on types
bool ksu_type(struct policydb *db, const char *name, const char *attr);
bool ksu_attribute(struct policydb *db, const char *name);
bool ksu_permissive(struct policydb *db, const char *type);
bool ksu_enforce(struct policydb *db, const char *type);
bool ksu_typeattribute(struct policydb *db, const char *type, const char *attr);
bool ksu_exists(struct policydb *db, const char *type);

// Access vector rules
bool ksu_allow(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);
bool ksu_deny(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);
bool ksu_auditallow(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);
bool ksu_dontaudit(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);

// Extended permissions access vector rules
bool ksu_allowxperm(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *range);
bool ksu_auditallowxperm(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *range);
bool ksu_dontauditxperm(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *range);

// Type rules
bool ksu_type_transition(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *def,
                         const char *obj);
bool ksu_type_change(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *def);
bool ksu_type_member(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *def);

// File system labeling
bool ksu_genfscon(struct policydb *db, const char *fs_name, const char *path, const char *ctx);

#endif
