#!/usr/bin/awk -f

NR == FNR {
    label[FNR] = $0;
    next
}

{
    printf("%d ", label[FNR])
}
{
    for (i=1;i<=NF;i++)
        printf("%d:%f ", i, $i)
    print ""
}
