<script>
    import Tooltip from "./Tooltip.svelte";

    /** @type {any} */
    export let entry;

    const date = new Date(entry.date)
</script>

<div class="entry">
    <div class="title">
        <Tooltip text="Id'et for personen der sendte dataene.">
            <h2>{entry.id}</h2>
        </Tooltip>
        <p>{date}</p>
    </div>
    <Tooltip text="Serienummeret på enheden der sendte dataene.">
        <p>Unit: {entry.serial}</p>
    </Tooltip>
    <p>Puls: {entry.data.pulse}</p>
</div>

<style>
    .entry {
        background-color: lightgray;
        padding: 1rem 1.3rem;
        border-radius: 1rem;
    }

    .title {
        display: flex;
        flex-direction: column;
        gap: 0;
        padding: 0;
        margin: 0;
    }

    .title h2, .title p {
        margin: 0;
    }

    
</style>