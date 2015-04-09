#include <stdio.h>
#include "debug.h"
#include "version.h"

#ifdef GENSTR_GIT_HASH
const char * genstr_git_hash = GENSTR_GIT_HASH;
#endif

GitVersionControlTypeDef GitVersionControl =
{
#ifdef GENSTR_GIT_BRANCH
  .head_branch = GENSTR_GIT_BRANCH,
#else
  .head_branch = 0,
#endif


#ifdef GENSTR_GIT_CAUTHOR
  .head_cauthor = GENSTR_GIT_CAUTHOR,
#else
  .head_cauthor = 0,
#endif


#ifdef GENSTR_GIT_CDATE
  .head_cdate = GENSTR_GIT_CDATE,
#else
  .head_cdate = 0,
#endif


#ifdef GENSTR_GIT_HASH
  .head_hash = GENSTR_GIT_HASH,
#else
  .head_hash = 0,
#endif

  .repo_name = 0,
};

void PrintGitVersionControl(void)
{
  if (GitVersionControl.head_branch)
  {
    printf("branch: %s" NEW_LINE, GitVersionControl.head_branch);
  }

  if (GitVersionControl.head_hash)
  {
    printf("commit: %s" NEW_LINE, GitVersionControl.head_hash);

    if (GitVersionControl.head_cauthor)
    {
      printf("author: %s" NEW_LINE, GitVersionControl.head_cauthor);
    }

    if (GitVersionControl.head_cdate)
    {
      printf("date: %s" NEW_LINE, GitVersionControl.head_cdate);
    }
  }
}

