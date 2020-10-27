
function getRangeBand(channelIndex) {
    return {
        "mark": {
            "type": "errorband",
            "borders": {
                "opacity": 0.3,
                "strokeDash": [3, 3]
            }
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channelIndex
                }
            }
        ],
        "encoding": {
            "x": {
                "field": "dim",
                "type": "ordinal"
            },
            "y": {
                "field": "min",
                "type": "quantitative"
            },
            "y2": {
                "field": "max",
                "type": "quantitative"
            },
            "color": {
                "value": {
                    "signal": `color${channelIndex}`
                }
            }
        }
    }
}

function getLineMark(channel, opacity, strokeWidth, strokeDash) {
    return {
        "mark": {
            "type": "line",
            "strokeWidth": strokeWidth,
            "strokeDash": strokeDash,
            "opacity": opacity
        },
        "transform": [
            {
                "filter": {
                    "field": "chn",
                    "equal": channel.index
                }
            }
        ],
        "encoding": {
            "x": {
                "field": "dim",
                "type": "nominal"
            },
            "y": {
                "field": "agg",
                "type": "quantitative",
            },
            "color": {
                "value": channel.color
            }
        }
    }
}

function addChannel(design, channel) {
    design.data.values = design.data.values.concat(channel.dimensions);

    if (channel.profileType > 0)
        design.layer.push(getLineMark(channel, 1, 1, []));

    if (channel.showRange)
        design.layer.push(getRangeBand(channelIndex));
}

function getDesign(configuration) {

    let design = {
        "$schema": "https://vega.github.io/schema/vega-lite/v4.json",
        "width": "container",
        "height": "container",
        "autosize":
        {
            "type": "fit",
            "resize": true,
        },
        "signals": [],
        "data": {
            "name": "dimensions",
            "values": []
        },
        "layer": []
    }

    for (channelName in configuration.channels) {
        addChannel(design, configuration.channels[channelName]);
    }

    return design;
}