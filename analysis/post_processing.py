import pandas as pd
import uproot
import argparse

def read_root_file(
        fn,
        treename='Hits'
        ):

    with uproot.open(fn) as f:
        tree = f[f"{treename}"]
        branches = ['eventID',
                    'pdgID',
                    'isEntry',
                    'postProcName',
                    'creatorProc',
                    'fX1', 
                    'fY1', 
                    'fZ1',
                    'fX2', 
                    'fY2', 
                    'fZ2', 
                    'fK1',
                    'fK2',
                    'fEdep'
                    ]
        
        data = tree.arrays(
            branches, 
            filter_name='nsteps', 
            library='np'
            )

    return data

parser = argparse.ArgumentParser(description="Process ROOT file")
parser.add_argument("--rootfile", type=str,  help="Rootfile")
parser.add_argument("--outfile", type=str, default='output.parquet', help="Output file")

args = parser.parse_args()

print(f"Processing rootfile ==> {args.rootfile}")
data = read_root_file(args.rootfile)
df = pd.DataFrame(data)

# Sum edep per event in one pass
edep_by_event = df.groupby("eventID")["fEdep"].sum()

# Keep only events with total edep > 0
good_events = edep_by_event[edep_by_event > 0].index

# Filter original df
mask = df["eventID"].isin(good_events)
eDf = df.loc[mask].copy()

# Now make the df smaller:
cat_cols = ["creatorProc", "postProcName"]
for c in cat_cols:
    eDf[c] = eDf[c].astype("category")

for c in eDf.select_dtypes(include=["int64"]).columns:
    eDf[c] = pd.to_numeric(eDf[c], downcast="integer")
for c in df.select_dtypes(include=["float64"]).columns:
    eDf[c] = pd.to_numeric(eDf[c], downcast="float")

eDf["isEntry"] = eDf["isEntry"].astype("bool")

eDf.to_parquet(args.outfile)
print("Saved!")
