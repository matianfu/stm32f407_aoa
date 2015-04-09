#ifndef _VERSION_H_
#define _VERSION_H_

/*
 * matianfu@gmail.com
 * 2015-04-09
 *
 * How to implement this.
 *
 * 1. In Eclipse, add an Environment Variable (NOT Build Variable), such as GENSTR_GIT,
 *    add to all configurations, empty value.
 * 2. In Eclipse, project build settings, UNCHECK Expand Env Variable Refs in Makefile option.
 *    This option will expand Env Variables into makefile, preventing make to expand them itself.
 * 3. In Eclipse, project build settings, Behavior tabs, change target 'all' to 'all+'.
 * 4. In Eclipse, choose version control file, such as version.c, add ${GENSTR_GIT} to its
 *    compiler options.
 * 5. Create a makefile.targets (or makefile.defs, makefile.init) file.
 * 6. In this makefile, which will be included in auto-generated makefile, add expressions to
 *    set value to GENSTR_GIT (make) Env Variable, let it expand to gcc -D options.
 * 7. In this makefile, create something like:
 *
 *    .PHONY: genstr_git all+
 *
 *    genstr_git:
 *        # ${GENSTR_GIT}
 *        rm -f Src/version.o
 *        rm -f stm32f407_aoa
 *
 *
 *    all+: genstr_git all
 *
 *    all+ is the make target. It requires the phony target genstr_git, so make will build this
 *    target each time. There's no need to update env variables in command. Simple assignment will
 *    do the job. But removing built target files are required. This is the simplest way to force
 *    make to rebuild new object and target.
 *
 * 8. You can also trigger the action through pre-build settings in Eclipse, but this way will use
 *    different targets for build in generated makefile. Using makefile to control the process is
 *    more flexbile.
 *
 *    NOTE: if you use pre-build settings, makefile.targets takes no effect. Don't know how to add
 *          customization in this situation.
 *
 * 9. See version.c code to see how to use compiler defined preprocessor macro to generate constant
 *    strings in codes. The #ifdef/#else/#endif directives are not required by compiler. But CDT
 *    will complain since it can not see the macro defined in make file.
 *
 */

typedef struct
{
  const char* head_branch;
  const char* head_cauthor;
  const char* head_cdate;
  const char* head_hash;
  const char* repo_name;
} GitVersionControlTypeDef;

extern GitVersionControlTypeDef GitVersionControl;

void PrintGitVersionControl(void);

#endif
