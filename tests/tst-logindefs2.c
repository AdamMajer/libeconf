#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "libeconf.h"

/* Test case:
   Open default logindefs.data from util-linux and try out if we can read
   all entries
*/

int
main(void)
{
  econf_file *key_file = NULL;
  char **keys;
  size_t key_number;
  econf_err error;

  if ((error = econf_readFile (&key_file, TESTSDIR"tst-logindefs2-data/logindefs.data", "= \t", "#")))
    {
      fprintf (stderr, "ERROR: couldn't read configuration file: %s\n", econf_errString(error));
      return 1;
    }

  if ((error = econf_getKeys(key_file, NULL, &key_number, &keys)))
    {
      fprintf (stderr, "Error getting all keys: %s\n", econf_errString(error));
      econf_free(key_file);
      return 1;
    }
  if (key_number == 0)
    {
      fprintf (stderr, "No keys found?\n");
      econf_free(key_file);
      return 1;
    }
  for (size_t i = 0; i < key_number; i++)
    {
      char *value = NULL;
      econf_getStringValue(key_file, NULL, keys[i], &value);
      printf ("%lu: %s: '%s'\n", i, keys[i], value);
      if (value != NULL)
	free (value);
    }

  int retval = 0;
  char *strval = NULL;
  econf_getStringValue (key_file, NULL, "STRING", &strval);
  if (strval == NULL || strcmp (strval, "this_is_string") != 0)
    {
      fprintf (stderr, "ERROR: %s, expected: %s, got: '%s'\n",
	       "STRING", "this_is_string", strval?strval:"NULL");
      retval = 1;
    }
  free (strval);

  int intval = 0;
  econf_getIntValue (key_file, NULL, "NUMBER", &intval);
  if (intval == 0 || intval != 123456)
    {
      fprintf (stderr, "ERROR: %s, expected: %i, got: %i\n",
	       "NUMBER", 123456, intval);
      retval = 1;
    }

  bool boolval = false;
  econf_getBoolValue (key_file, NULL, "BOOLEAN", &boolval);
  if (boolval != true)
    {
      fprintf (stderr, "ERROR: %s, expected: %i, got: %i\n",
	       "BOOLEAN", true, boolval);
      retval = 1;
    }

  strval = NULL;
  econf_getStringValue (key_file, NULL, "EMPTY", &strval);
  if (strval == NULL || strcmp (strval, "") != 0)
    {
      fprintf (stderr, "ERROR: %s, expected: '%s', got: '%s'\n",
	       "EMPTY", "", strval?strval:"NULL");
      retval = 1;
    }

  econf_free (keys);
  econf_free (key_file);

  return retval;
}