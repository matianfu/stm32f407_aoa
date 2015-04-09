#ifndef _VERSION_H_
#define _VERSION_H_

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
