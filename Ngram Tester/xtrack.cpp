#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "xtrack.h"

#if defined(DEBUG) || defined(_DEBUG)

/* Simple linked list node to track information about a memory allocation */
typedef struct XTRACK_REF XTRACK_REF;
struct XTRACK_REF
{
    void*           mem;
    unsigned int    line;
    const char*     file;
    int             seq;
    XTRACK_REF*     pNext;
};

/* Data storage to track allocations */
XTRACK_REF **   xt_pBuckets = NULL;
int             xt_count = 0;
int             xt_buckets = 0;

/* The pointer to the head and tail of our linked list */
XTRACK_REF *    xt_pOldFirst = NULL;
XTRACK_REF *    xt_pOldLast = NULL;
int             xt_oldCount = 0;

/* A sequence number, can be useful for debugging */
int             xt_seq = 0;

/* Is the XTRACK layer running? */
int             xt_running = 0;

/* What do for warnings? */
void XTrack_Warn(const char* line)
{
    /* Just dump out the information using printf */
    printf("%s", line);
}

/* What to do when we want to break into the debugger? */
void XTrack_Break()
{
    /* Don't bother right now */
}

/* Start the XTRACK layer */
void XTrack_Startup()
{
    xt_running = -1;
}

/* Simple helper to print out an error message */
void XTrack_Error(const char * format, ...)
{
    char * info = (char*)malloc(1000);
    va_list args;
    va_start(args, format);
#ifdef WIN32
    vsprintf_s(info, 1000, format, args);
#else
    vsprintf(info, format, args);
#endif
    XTrack_Warn(info);
    va_end(args);
    free(info);
}

/* Stop the XTRACK layer, reporting on anything still being tracked */
void XTrack_Cleanup()
{
    int i = 0;
    XTRACK_REF * cur = NULL;

    if (xt_running)
    {
        xt_running = 0;

        if (xt_count)
        {
            /* We have memory leaks, dump all the information */
            XTrack_Warn("---- Dump of all memory leaks ----\n");

            for (i = 0; i < xt_buckets; i ++)
            {
                cur = xt_pBuckets[i];

                while (cur)
                {
                    XTrack_Error("%s(%d): Seq #%d, Never untracked!\n",
                        cur->file, cur->line, cur->seq);

                    cur = cur->pNext;
                }
            }

            XTrack_Warn("---- End of dump ----\n");
            XTrack_Break();
        }
        else
        {
            /* You're such a good developer! */
            XTrack_Warn("---- No memory leaks detected ----\n");
        }

        /* Clean up our memory */
        for (i = 0; i < xt_buckets; i ++)
        {
            cur = xt_pBuckets[i];

            while (cur)
            {
                XTRACK_REF * next = cur->pNext;
                free(cur);
                cur = next;
            }
        }

        free(xt_pBuckets);

        cur = xt_pOldFirst;
        while (cur)
        {
            XTRACK_REF * next = cur->pNext;
            free(cur);
            cur = next;
        }
    }
    else
    {
        XTrack_Warn("---- WARNING: Attempt to stop the XTRACK layer without starting it! ----\n");
        XTrack_Break();
    }
}

/* Produce a hash for a pointer, to map it to a bucket */
int XTrack_ID(void * lpvoid) 
{
	unsigned long long key = (unsigned long long)lpvoid;
	long long bucket = 1;
	long long next_bucket = 0;

	while (next_bucket < xt_buckets) 
	{
		bucket = next_bucket;
		key = key * 2862933555777941757ULL + 1;
		next_bucket = (long long)((bucket + 1) * ((double)(1LL << 31) / (double)((key >> 33) + 1)));
	}

	return (int)bucket;
}

/* Given a pointer , find the XTRACK_REF in a bucket, or return NULL if not found */
XTRACK_REF* XTrack_Get(void * value)
{
    if (xt_count > 0)
    {
        int id = XTrack_ID(value);
        XTRACK_REF * cur = xt_pBuckets[id];
        while (cur)
        {
            if (cur->mem == value)
            {
                return cur;
            }
            cur = cur->pNext;
        }
    }
    return NULL;
}

/* Track a new item, storing it by item->cur */
void XTrack_Add(XTRACK_REF* item)
{
    int id = 0;

	/* If we have too many items, increase the number of buckets */
    if (xt_count >= xt_buckets)
    {
        if (xt_buckets == 0)
        {
			/* Special case, initial memory setup */
            size_t len = 100 * sizeof(XTRACK_REF*);
            xt_buckets = 100;
            xt_pBuckets = (XTRACK_REF**)malloc(len);
            memset(xt_pBuckets, 0, len);
        }
        else
        {
			/* Create an array twice as big, and put everything in the new array */
            XTRACK_REF ** copy = NULL;
            XTRACK_REF * cur = NULL;
            int i = 0;
            int oldBuckets = xt_buckets;
            size_t len = xt_buckets * 2 * sizeof(XTRACK_REF*);

            copy = (XTRACK_REF**)malloc(len);
            memset(copy, 0, len);

            xt_buckets *= 2;

            for (i = 0; i < oldBuckets; i ++)
            {
                cur = xt_pBuckets[i];
                while (cur)
                {
                    XTRACK_REF* next = cur->pNext;
                    id = XTrack_ID(cur->mem);

                    cur->pNext = copy[id];
                    copy[id] = cur;

                    cur = next;
                }
            }

            free(xt_pBuckets);
            xt_pBuckets = copy;
        }
    }

	/* Store the item in its bucket */
    id = XTrack_ID(item->mem);
    item->pNext = xt_pBuckets[id];
    xt_pBuckets[id] = item;
    xt_count++;
}

/* Remove an item from the corresponding bucket */
void XTrack_Remove(void * value)
{
    int id = XTrack_ID(value);
    XTRACK_REF * cur = xt_pBuckets[id];
    XTRACK_REF * last = NULL;

    while (cur)
    {
        if (cur->mem == value)
        {
            if (last)
            {
                last->pNext = cur->pNext;
            }
            else
            {
                xt_pBuckets[id] = cur->pNext;
            }

            xt_count--;

            break;
        }

        last = cur;
        cur = cur->pNext;
    }
}

/* Track a pointer or pointer like thing, returns the pointer being tracked */
void* XTrack_Track(void * value, unsigned int line, const char * file)
{
    if (xt_running)
    {
        if (value)
        {
			XTRACK_REF * item = NULL;

            if (XTrack_Get(value))
            {
                XTrack_Error("%s(%d): Error!  Attempt to double track memory!\n", file, line);
                XTrack_Break();
            }

            /* Build up the node of information, only for non-NULL */
            item = (XTRACK_REF *)malloc(sizeof(XTRACK_REF));
            xt_seq++; /* Not thread safe, but it doesn't matter too much */
            item->file = file;
            item->line = line;
            item->mem = value;
            item->pNext = NULL;
            item->seq = xt_seq;

            XTrack_Add(item);
        }
    }
    else
    {
        if (value)
        {
            XTrack_Error("%s(%d): Error!  Attempt to track memory before XTRACK_START()!\n", file, line);
            XTrack_Break();
        }
    }

    return value;
}

/* Remove the track for a pointer or pointer like thing, returns the pointer being untracked */
void* XTrack_Untrack(void * value, unsigned int line, const char * file)
{
    if (xt_running)
    {
        if (value)
        {
            /* Find the bit to be removed from the list */
            XTRACK_REF * cur = XTrack_Get(value);

            if (!cur)
            {
                int found = 0;

                cur = xt_pOldFirst;
                while ((found == 0) && (cur))
                {
                    if (cur->mem == value)
                    {
                        found = 1;
                    }
                    else
                    {
                        cur = cur->pNext;
                    }
                }

                /* This means we're attempting to XUNTRACK something not marked via XTRACK, or XUNTRACK
                    the same value twice.  Both are bad. */

                if (found)
                {
                    XTrack_Error("%s(%d): Error!  Attempt to uninitialize already freed memory, was " 
                        "originally allocated on %s(%d), seq %d!\n", file, line, cur->file, cur->line, cur->seq);
                }
                else
                {
                    XTrack_Error("%s(%d): Error!  Attempt to uninitialize unallocated memory!\n", file, line);
                }

                XTrack_Break();
            }
            else
            {
                XTrack_Remove(value);

                /* Add this node to our linked list for old items */
                if (xt_pOldFirst == NULL)
                {
                    xt_pOldFirst = cur;
                    xt_pOldLast = cur;
                }
                else
                {
                    xt_pOldLast->pNext = cur;
                    xt_pOldLast = cur;
                }

                cur->pNext = NULL;

                xt_oldCount ++;

                /* And limit the size of the old list */
                while (xt_oldCount > 1000)
                {
                    cur = xt_pOldFirst->pNext;
                    free(xt_pOldFirst);
                    xt_pOldFirst = cur;
                    xt_oldCount--;
                }
            }
        }
    }
    else
    {
        if (value)
        {
            XTrack_Error("%s(%d): Error!  Attempt to free memory before XTRACK_START()!\n", file, line);
            XTrack_Break();
        }
    }

    return value;
}

#endif
