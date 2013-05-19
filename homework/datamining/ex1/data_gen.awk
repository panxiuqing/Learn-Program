#!/usr/bin/awk -f

BEGIN{
    print "data = {"
}

{
    printf("'%s':", $1)
}

{
    printf("set([");
    for (i=2; i<NF; i++)
        printf("'%s', ", $i);
    printf("'%s']),\n", $NF)
}

END{
    print "}"
}
