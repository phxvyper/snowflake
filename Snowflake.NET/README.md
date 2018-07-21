You can obtain this package through nuget or by just building this project. .NET Standard 2.0 is the only dependency.

## Usage

```cs
// id obtained through whatever means your infrustructure demands. It should be unique
// in the context of your infrastructure's set of Snowflake workers.
var workerid = 25;

// instantiate a new worker with the determined workerid.
var worker = new SnowflakeWorker(workerid);

if (worker.Next(out ulong snowflake) == SnowflakeError.None)
{
    // you now have a unique snowflake to work with.
    // Do with it as you please.
}
else
{
    // there was an error obtaining a unique snowflake!
    // its up to you how you want to handle this.
}
```
