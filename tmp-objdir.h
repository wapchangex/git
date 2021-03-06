#ifndef TMP_OBJDIR_H
#define TMP_OBJDIR_H

/*
 * This API allows you to create a temporary object directory, advertise it to
 * sub-processes via GIT_OBJECT_DIRECTORY and GIT_ALTERNATE_OBJECT_DIRECTORIES,
 * and then either migrate its object into the main object directory, or remove
 * it. The library handles unexpected signal/exit death by cleaning up the
 * temporary directory.
 *
 * Example:
 *
 *	struct tmp_objdir *t = tmp_objdir_create("incoming");
 *	if (!run_command_v_opt_cd_env(cmd, 0, NULL, tmp_objdir_env(t)) &&
 *	    !tmp_objdir_migrate(t))
 *		printf("success!\n");
 *	else
 *		die("failed...tmp_objdir will clean up for us");
 *
 */

struct tmp_objdir;

/*
 * Create a new temporary object directory with the specified prefix;
 * returns NULL on failure.
 */
struct tmp_objdir *tmp_objdir_create(const char *prefix);

/*
 * Return a list of environment strings, suitable for use with
 * child_process.env, that can be passed to child programs to make use of the
 * temporary object directory.
 */
const char **tmp_objdir_env(const struct tmp_objdir *);

/*
 * Finalize a temporary object directory by migrating its objects into the main
 * object database, removing the temporary directory, and freeing any
 * associated resources.
 */
int tmp_objdir_migrate(struct tmp_objdir *);

/*
 * Destroy a temporary object directory, discarding any objects it contains.
 */
int tmp_objdir_destroy(struct tmp_objdir *);

/*
 * Add the temporary object directory as an alternate object store in the
 * current process.
 */
void tmp_objdir_add_as_alternate(const struct tmp_objdir *);

/*
 * Replaces the writable object store in the current process with the temporary
 * object directory and makes the former main object store an alternate.
 * If will_destroy is nonzero, the object directory may not be migrated.
 */
void tmp_objdir_replace_primary_odb(struct tmp_objdir *, int will_destroy);

/*
 * If the primary object database was replaced by a temporary object directory,
 * restore it to its original value while keeping the directory contents around.
 * Returns NULL if the primary object database was not replaced.
 */
struct tmp_objdir *tmp_objdir_unapply_primary_odb(void);

/*
 * Reapplies the former primary temporary object database, after potentially
 * changing its relative path.
 */
void tmp_objdir_reapply_primary_odb(struct tmp_objdir *, const char *old_cwd,
		const char *new_cwd);


#endif /* TMP_OBJDIR_H */
